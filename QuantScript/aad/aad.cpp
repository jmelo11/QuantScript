#include "aad.h"

namespace QuantScript {
	std::vector<AADNode> tape;
	std::ostream& operator<<(std::ostream& os, const AADNumber& rhs) {
		os << rhs.value;
		return os;
	};
	std::string to_string(AADNumber& arg){
		return std::to_string(arg.value);
	};	
}