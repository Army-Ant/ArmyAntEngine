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

#ifndef C_AA_NEURON_H_2016_2_23
#define C_AA_NEURON_H_2016_2_23

#include "C_AADefine.h"
#include "AA_start.h"

#ifdef _cplusplus
extern "C" {
#endif

	typedef uint32 AA_CNeuronAlgorithm;
	typedef double(*AA_Neuron_ActiveFunc)(double input);

	ARMYANT_CLIB_API double AA_Math_Liner(double input, double slope, double bias);
	ARMYANT_CLIB_API double AA_Math_Ramp(double input, double width, double height);
	ARMYANT_CLIB_API double AA_Math_Threshold(double input, double amplitude, double threshold);
	ARMYANT_CLIB_API double AA_Math_Sigmoid(double input, double coef);
	ARMYANT_CLIB_API double AA_Math_DoubleSigmoid(double input, double coef);

	/*
		Usage:

		double My_Liner(double input)
		{
			double slope = 1; // Your setting slope
			double bias = 0; // Your setting bias
			return AA_Math_Liner(input, slope, bias);
		}

		AA_CNeuronAlgorithm object = AA_NeuronAlgorithm_Create(My_Liner, 0);

	*/

	ARMYANT_CLIB_API AA_CNeuronAlgorithm AA_NeuronAlgorithm_Create(AA_Neuron_ActiveFunc activationFunc, double threshold);
	ARMYANT_CLIB_API AA_CNeuronAlgorithm AA_NeuronAlgorithm_Clone(AA_CNeuronAlgorithm value);
	ARMYANT_CLIB_API void AA_NeuronAlgorithm_Release(AA_CNeuronAlgorithm value);

	ARMYANT_CLIB_API BOOL AA_NeuronAlgorithm_JoinActive(AA_CNeuronAlgorithm value, double input, double weight);
	ARMYANT_CLIB_API BOOL AA_NeuronAlgorithm_LeaveActive(AA_CNeuronAlgorithm value, double input, double weight);
	ARMYANT_CLIB_API BOOL AA_NeuronAlgorithm_ClearActive(AA_CNeuronAlgorithm value);
	ARMYANT_CLIB_API double AA_NeuronAlgorithm_GetOutput(AA_CNeuronAlgorithm value);
	ARMYANT_CLIB_API double AA_NeuronAlgorithm_GetAllActive(AA_CNeuronAlgorithm value);

	ARMYANT_CLIB_API BOOL AA_NeuronAlgorithm_SetActivationFunction(AA_CNeuronAlgorithm value, AA_Neuron_ActiveFunc func);
	ARMYANT_CLIB_API double AA_NeuronAlgorithm_GetThreshold(AA_CNeuronAlgorithm value);
	ARMYANT_CLIB_API BOOL AA_NeuronAlgorithm_SetThreshold(AA_CNeuronAlgorithm value, double threshold);

#ifdef _cplusplus
}

namespace ArmyAnt {
template <class T_Weight> class NeuronAlgorithm;
ARMYANTLIB_API ArmyAnt::NeuronAlgorithm<double>*  NeuronAlgorithm_GetCppObject(AA_CNeuronAlgorithm value);
}

#endif

#endif // C_AA_NEURON_H_2016_2_23