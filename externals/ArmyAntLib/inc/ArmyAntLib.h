/*
* Copyright (c) 2015 ArmyAnt
* 版权所有 (c) 2015 ArmyAnt
*
* Licensed under the BSD License, Version 2.0 (the License);
* 本软件使用BSD协议保护, 协议版本:2.0
* you may not use this file except in compliance with the License.
* 使用本开源代码文件的内容, 视为同意协议
* You can read the license content in the file "ARMYANT.COPYRIGHT.BSD_LICENSE.MD" at the root of this project
* 您可以在本项目的根目录找到名为"ARMYANT.COPYRIGHT.BSD_LICENSE.MD"的文件, 来阅读协议内容
* You may also obtain a copy of the License at
* 您也可以在此处获得协议的副本:
*
*     http://opensource.org/licenses/BSD-3-Clause
*
* Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an AS IS BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* 除非法律要求或者版权所有者书面同意,本软件在本协议基础上的发布没有任何形式的条件和担保,无论明示的或默许的.
* See the License for the specific language governing permissions and limitations under the License.
* 请在特定限制或语言管理权限下阅读协议
*/

#ifndef ARMYANT_LIB_H_2016_3_19
#define ARMYANT_LIB_H_2016_3_19

#include "AADefine.h"
#include "AA_start.h"

namespace ArmyAnt {

namespace Infos {

union Version
{
public:
	uint32 ver;
	struct VerChar
	{
		uint8 bigVer;
		uint8 smallVer;
		uint8 patchVer;
		uint8 testVer;

		VerChar(uint8 bigVer, uint8 smallVer, uint8 patchVer, uint8 testVer);
	}sver;

	Version(uint32 ver);
	Version(uint8 bigVer, uint8 smallVer, uint8 patchVer, uint8 testVer);

public:
	// The version of our library and other referenced libraries
	// Get our library's version without param
	// Avaliable param values: boost python
	static const Version GetVersion(const char* obj = nullptr);
};

class ARMYANTLIB_API Enviroment
{
public:
	enum class OS : int8
	{
		// Big class
		Unix = 0b00000000,
		Windows = 0b00000001,
		// Medium class
		// For Unix
		Linux = 0b00000010,
		BSD = 0b00000100,
		Solaris = 0b00000110,
		// For Windows
		WindowsPhone = 0b00000011,
		// Small class
		// For Linux
		Fedora = 0b00100010,
		Ubuntu = 0b00001010,
		YunOS = 0b00101010,
		Debian = 0b00010010,
		RedHat = 0b00110010,
		ChromeOS = 0b00011010,
		Android = 0b00111010,
		CentOS = 0b01000010,
		SuSE = 0b01001010,
		// For BSD
		FreeBSD = 0b00010100,
		OSX = 0b00001100,
		iOS = 0b00011100,
	};

	enum class BITS : int8
	{
		_8bits,
		_16bits,
		_32bits,
		_64bits,
		_128bits
	};

	enum class DevLanguage : int8
	{
		Unknown,
		Assembly_Intel,
		Assembly_AMD,
		Assembly_ARM,
		COBOL,
		Fortrain,
		Basic,
		C,
		SmartTalk,
		Matlab,
		Perl,
		Shell,
		CPlusPlus,
		VisualBasic,
		Batch,
		Java,
		Javascript,
		PHP,
		ObjectiveC,
		CSharp,
		Python,
		Lua,
		Ruby,
		Go,
		FSharp,
		Swift,
		ActionScript,
		Delphi,
		R,
		D,
		Groovy,
		Lisp,
		Scala,
		Ada
	};

	static OS GetOSType();
	static Version GetOSVersion();
	static BITS GetOSBits();
};

} // namespace Infos

namespace Utils {

	namespace Log {
		void Log_Debug(const char* msg);
		void Log_Error(const char* msg);
	}

	namespace Math {
		double GetRandom(double min, double max);
		bool GetRoll(double percent);
		uint32 GetMultiRoll(double percent);
	}
}

} // namespace ArmyAnt

#endif // ARMYANT_LIB_H_2016_3_19