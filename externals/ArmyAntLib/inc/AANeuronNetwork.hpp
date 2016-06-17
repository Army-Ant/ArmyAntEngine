#ifndef AA_NEURON_NETWORK_HPP
#define AA_NEURON_NETWORK_HPP

#include <array>
#include "AAStateMachine.hpp"
#include "AANeuron.hpp"
#include "AADigraph.hpp"

namespace ArmyAnt {

	template <class T_Val = uint32>
	class NeuronNetwork : public Digraph<T_Val, uint32> 
	{
	public:
		enum class Type : uint8 
		{
			Unknown,
			Feedforward,
			Feedback,
			SelfOrganizing,

		};
	public:
		NeuronNetwork(std::array<T_Val> neuronNumAndFloor, T_Val minValue, T_Val maxValue, NeuronAlgorithm::ActiveFunc);
		~NeuronNetwork();
		NeuronNetwork Clone();
		static NeuronNetwork Create(std::array<T_Val> neuronNumAndFloor, T_Val min, T_Val max, NeuronAlgorithm::ActiveFunc);
		AA_FORBID_COPY_CTOR(NeuronNetwork);
		AA_FORBID_ASSGN_OPR(NeuronNetwork);

	public:
		const std::array<T_Val> Active(const std::array<T_Val>& input);
		bool Forget();

	private:
		static double GetSmallRandomNum();

	};

}

#endif // AA_NEURON_NETWORK_HPP
