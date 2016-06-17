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

#ifndef AA_NEURON_HPP_2016_1_21
#define AA_NEURON_HPP_2016_1_21

/*	* @ author			: Jason
	* @ date			: 01/21/2016
	* @ nearly update	: 01/21/2016
	* @ small version	: 0.1
	* @ summary			: 神经元
	* @ uncompleted		: all
	* @ untested		:
	* @ tested			:
	*/

#include "AADefine.h"
#include "AADigraph.hpp"
#include "AAStateMachine.hpp"
#include <iostream>


namespace ArmyAnt {

template <class T_Weight = double>
class NeuronAlgorithm
{
public:
	typedef std::function<T_Weight(T_Weight)> ActiveFunc;

public: // Some normal activation functions

	/*	Liner function
	 *	线性函数, 默认值为单位正比例函数
	 */
	inline static ActiveFunc GetLinerFunc(T_Weight  slope = T_Weight(1), T_Weight bias = T_Weight(0));

	/*	Ramp function
	 *	对称斜坡函数 , 默认值为单位斜坡函数
	 */
	inline static ActiveFunc GetRampFunc(T_Weight  width = T_Weight(1), T_Weight height = T_Weight(1));

	/*	Step function (Threshold function)
	 *	阶跃函数 (门阀函数) , 默认值为单位阶跃函数
	 */
	inline static ActiveFunc GetThresholdFunc(T_Weight  amplitude = T_Weight(1), T_Weight threshold = T_Weight(0));

	/*	Sigmoid function
	 *	S形函数
	 */
	inline static ActiveFunc GetSigmoidFunc(T_Weight  coef = T_Weight(1));

	/*	Double-sigmoid function
	 *	双S形函数
	 */
	inline static ActiveFunc GetDoubleSigmoidFunc(T_Weight  coef = T_Weight(1));


public:
	NeuronAlgorithm(ActiveFunc activationFunc, T_Weight threshold = T_Weight(0));
	NeuronAlgorithm(const NeuronAlgorithm&value);
	NeuronAlgorithm<T_Weight>&operator=(const NeuronAlgorithm&value);
	virtual ~NeuronAlgorithm();

public:
	// 传入信号
	bool JoinActive(T_Weight input, T_Weight weight);
	// 取消信号
	bool LeaveActive(T_Weight input, T_Weight weight);
	// 重置输入
	bool ClearActive();
	// 求输出
	T_Weight GetOutput() const;
	// 求信号权值总和
	T_Weight GetAllActive() const;

	// 更改激活函数
	bool SetActivationFunction(ActiveFunc func);
	// 查看阀值
	T_Weight GetThreshold()const;
	// 修改阀值
	bool SetThreshold(T_Weight value);

public:
	inline NeuronAlgorithm<T_Weight>&operator<<(T_Weight inputweight);
	inline NeuronAlgorithm<T_Weight>&operator>>(T_Weight inputweight);
	inline operator T_Weight();

protected:
	T_Weight threshold = T_Weight(0);	//阀值
	ActiveFunc activationFunc = nullptr; //激活函数

private:
	T_Weight inputAll = 0;
};


/*************************** Source Code : NeuronAlgorithm *********************************************/


template<class T_Weight>
inline typename NeuronAlgorithm<T_Weight>::ActiveFunc NeuronAlgorithm<T_Weight>::GetLinerFunc(T_Weight slope, T_Weight bias)
{
	return [=](T_Weight input){return T_Weight(slope*input + bias);};	// y=kx+b
}

template<class T_Weight>
inline typename NeuronAlgorithm<T_Weight>::ActiveFunc NeuronAlgorithm<T_Weight>::GetRampFunc(T_Weight width, T_Weight height)
{
	return [=](T_Weight input)
	{
		if(input > width)
			return height;
		if(input < -width)
			return -height;
		return T_Weight(input*height / width);
	};
}

template<class T_Weight>
inline typename NeuronAlgorithm<T_Weight>::ActiveFunc NeuronAlgorithm<T_Weight>::GetThresholdFunc(T_Weight amplitude, T_Weight threshold)
{
	return [=](T_Weight input){return input > threshold ? amplitude : T_Weight(0);};
}

template<class T_Weight>
inline typename NeuronAlgorithm<T_Weight>::ActiveFunc NeuronAlgorithm<T_Weight>::GetSigmoidFunc(T_Weight coef)
{
	return [=](T_Weight input){return 1 / (1 + pow(ArmyAnt::Constant::c_natrualBase, -coef*input));};
}

template<class T_Weight>
inline typename NeuronAlgorithm<T_Weight>::ActiveFunc NeuronAlgorithm<T_Weight>::GetDoubleSigmoidFunc(T_Weight coef)
{
	return [=](T_Weight input){return 2 / (1 + pow(ArmyAnt::Constant::c_natrualBase, -coef*input)) - 1;};
}

template<class T_Weight>
NeuronAlgorithm<T_Weight>::NeuronAlgorithm(ActiveFunc activationFunc, T_Weight threshold)
	: activationFunc(activationFunc), threshold(threshold)
{
	AAAssert(activationFunc != nullptr, );
}

template<class T_Weight>
NeuronAlgorithm<T_Weight>::NeuronAlgorithm(const NeuronAlgorithm & value)
	: activationFunc(value.activationFunc), threshold(value.threshold), inputAll(value.inputAll)
{
}

template<class T_Weight>
NeuronAlgorithm<T_Weight> & NeuronAlgorithm<T_Weight>::operator=(const NeuronAlgorithm & value)
{
	activationFunc = value.activationFunc;
	threshold = value.threshold;
	inputAll = value.inputAll;
	return *this;
}

template<class T_Weight>
NeuronAlgorithm<T_Weight>::~NeuronAlgorithm()
{
}

template<class T_Weight>
bool NeuronAlgorithm<T_Weight>::JoinActive(T_Weight input, T_Weight weight)
{
	inputAll += input*weight;
	return true;
}

template<class T_Weight>
bool NeuronAlgorithm<T_Weight>::LeaveActive(T_Weight input, T_Weight weight)
{
	inputAll -= input*weight;
	return true;
}

template<class T_Weight>
bool NeuronAlgorithm<T_Weight>::ClearActive()
{
	inputAll = 0;
	return true;
}

template<class T_Weight>
T_Weight NeuronAlgorithm<T_Weight>::GetOutput() const
{
	return activationFunc(inputAll - threshold);
}

template<class T_Weight>
T_Weight NeuronAlgorithm<T_Weight>::GetAllActive() const
{
	return inputAll;
}

template<class T_Weight>
bool NeuronAlgorithm<T_Weight>::SetActivationFunction(ActiveFunc func)
{
	if(func == nullptr)
		return false;
	activationFunc = func;
	return true;
}

template<class T_Weight>
T_Weight NeuronAlgorithm<T_Weight>::GetThreshold() const
{
	return threshold;
}

template<class T_Weight>
bool NeuronAlgorithm<T_Weight>::SetThreshold(T_Weight value)
{
	if(value < T_Weight(0))
		return false;
	threshold = value;
	return true;
}

template<class T_Weight>
inline NeuronAlgorithm<T_Weight> & NeuronAlgorithm<T_Weight>::operator<<(T_Weight inputweight)
{
	inputAll += inputweight;
	return*this;
}

template<class T_Weight>
inline NeuronAlgorithm<T_Weight> & NeuronAlgorithm<T_Weight>::operator>>(T_Weight inputweight)
{
	inputAll -= inputweight;
	return*this;
}

template<class T_Weight>
inline NeuronAlgorithm<T_Weight>::operator T_Weight()
{
	return GetOutput();
}

} // namespace ArmyAnt

#endif // AA_NEURON_HPP_2016_1_21
