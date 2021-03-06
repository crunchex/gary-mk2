/*
 * PWM.h
 * A C++ wrapper for the EHRPWM interface
 *
 *  Created on: May 27, 2013
 *      Author: Saad Ahmad ( http://www.saadahmad.ca )
 */

#ifndef PWM_H_
#define PWM_H_

#include <string>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <iostream>

const int US_TO_NS = 1000;
const int MS_TO_US = 1000;
const int MS_TO_NS = MS_TO_US * US_TO_NS;

//  Time to wait for module to be loaded and the sysfs interface setup
const long MODULE_DELAY_TIME_US = 100 * MS_TO_US; 

#define DEBUG_VERBOSE_OUTPUT 0

namespace PWM {
	template<class T> inline std::string ToString(const T & value)	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	inline void WriteToFile(const std::string & filePath, const std::string & value) {
#if DEBUG_VERBOSE_OUTPUT
		std::cout << "Writing: " << value << " to: " << filePath << std::endl;
#endif
		std::ofstream out;
		out.open(filePath.c_str());
		out.exceptions(std::ios::badbit);
		out << value << std::endl;
		out.close();
	}


	/* Helper functions to get us locations in the file system.
	 * They are used to manipulate the pwm driver through the /sys
	 * interface */
	std::string GetFullNameOfFileInDirectory(const std::string & dirName, const std::string & fileNameToFind);
	std::string GetCapeManagerSlotsPath();
	std::string GetOCPPath();
	int GetCapeManagerSlot(const std::string & moduleName);
	void LoadDeviceTreeModule(const std::string & name);
	void UnloadDeviceTreeModule(const std::string name);

	class Pin {
	public:
		enum RunStatus {
			Free = -2, WaitingForSetp, Disabled, Enabled,
		};
		enum Polarity {
			PolarityHigh = 0, PolarityLow,
		};

	private:
		std::string m_dutyFilePath;
		std::string m_periodFilePath;
		std::string m_polarityFilePath;
		std::string m_runFilePath;
		std::string m_pinName;

		long m_periodNS;
		long m_dutyNS;
		Polarity m_polarity;
		RunStatus m_runStatus;

	public:
		const std::string &GetDutyFilePath() const {
			return m_dutyFilePath;
		}
		const std::string &GetPeriodFilePath() const {
			return m_periodFilePath;
		}
		const std::string &GetPolarityFilePath() const {
			return m_polarityFilePath;
		} const std::string &GetPinName() const {
			return m_pinName;
		}
		const std::string &GetRunFilePath() const {
			return m_runFilePath;
		}

		const RunStatus &GetRunStatus() const {
			return m_runStatus;
		}

		const long &GetPeriodNS() const {
			return m_periodNS;
		}
		const Polarity &GetPolarity() const {
			return m_polarity;
		}
		const long &GetDutyNS() const {
			return m_dutyNS;
		}

	private:
		void WriteDutyNSToFile() {
			WriteToFile(GetDutyFilePath(), ToString(GetDutyNS()));
		}
		void WritePeriodNSToFile() {
			WriteToFile(GetPeriodFilePath(), ToString(GetPeriodNS()));
		}
		void WritePolarityToFile() {
			WriteToFile(GetPolarityFilePath(), GetPolarity() == PolarityHigh ? std::string("0") : std::string("1"));
		}

	public:
		void SetDutyNS(const long & dutyNS) {
			m_dutyNS = std::min(dutyNS, GetPeriodNS());
			if (GetRunStatus() == Enabled)
				WriteDutyNSToFile();
		}
		void SetDutyUS(const int &dutyUS) {
			SetDutyNS((long) dutyUS * US_TO_NS);
		}
		void SetDutyMS(const int &dutyMS) {
			SetDutyNS((long) dutyMS * MS_TO_NS);
		}
		void SetDutyPercent(const float &percent) {
			SetDutyNS(long(GetPeriodNS() * percent));
		}

		void SetPeriodNS(const long & periodNS) {
			if (GetRunStatus() == Enabled || GetRunStatus() == Disabled) {
				std::cout << "Trying to set the period but we need to release the PWM module first!" << std::endl;
				throw std::bad_exception();
				return;
			}
			m_periodNS = periodNS;
			if (GetRunStatus() == Enabled)
				WritePeriodNSToFile();
		}
		void SetPeriodUS(const int &periodUS) {
			SetPeriodNS((long) periodUS * US_TO_NS);
		}
		void SetPeriodMS(const int &periodMS) {
			SetPeriodNS((long) periodMS * MS_TO_NS);
		}

		void SetPolarity(const Polarity & polarity) {
			m_polarity = polarity;
			if (GetRunStatus() == Enabled)
				WritePolarityToFile();
		}

	private:
		void SetRunStatus(const RunStatus & newRunStatus) {
			if (newRunStatus != GetRunStatus()) {
				if (newRunStatus == Disabled) {
					WriteToFile(GetRunFilePath(), std::string("0"));
				}
				else if (newRunStatus == Enabled) {
					if (GetRunStatus() == Free) {
						InitPinFS();
					}
					// Force write the file values out
					WritePeriodNSToFile();
					WriteDutyNSToFile();
					WritePolarityToFile();

					WriteToFile(GetRunFilePath(), std::string("1"));
				} else if (newRunStatus == Free) {
					if (GetRunStatus() != Disabled) {
						SetRunStatus(Disabled);
					}
					UnloadDeviceTreeModule(GetPinName());
				}
			}
			m_runStatus = newRunStatus;
		}
	public:
		void Enable() {
			SetRunStatus(Enabled);
		}
		void Disable() {
			SetRunStatus(Disabled);
		}
		void Release() {
			SetRunStatus(Free);
		}

	public:
		~Pin() {
			Release();
		}
		Pin(const std::string & pinName, const long & periodNS = 20 * MS_TO_NS, const long & dutyNS = 1 * MS_TO_NS) :
			m_pinName(pinName) {
			// If the pin is already in use then we need to free it!
			if (GetCapeManagerSlot(GetPinName()) != -1)
				UnloadDeviceTreeModule(GetPinName());

			m_runStatus = WaitingForSetp;

			SetPeriodNS(periodNS);
			SetDutyNS(dutyNS);
			SetPolarity(PolarityHigh);

			InitPinFS();
		}

		void InitPinFS() {
			LoadDeviceTreeModule(std::string("am33xx_pwm"));
			std::string pinModule = std::string("sc_pwm_") + GetPinName();
			LoadDeviceTreeModule(pinModule);
			std::string pinInterfacePath = GetOCPPath() + GetFullNameOfFileInDirectory(GetOCPPath(), GetPinName()) + "/";
			m_dutyFilePath = pinInterfacePath + "duty";
			m_periodFilePath = pinInterfacePath + "period";
			m_polarityFilePath = pinInterfacePath + "polarity";
			m_runFilePath = pinInterfacePath + "run";


#if DEBUG_VERBOSE_OUTPUT
			std::cout << GetDutyFilePath() << std::endl;
			std::cout << GetPeriodFilePath() << std::endl;
			std::cout << GetPolarityFilePath() << std::endl;
			std::cout << GetRunFilePath() << std::endl;
#endif
		}
	};
}

#endif /* PWM_H_ */
