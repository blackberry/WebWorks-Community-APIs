/*
 * AES.hpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#ifndef AES_HPP_
#define AES_HPP_

#include "provider.hpp"

namespace webworks {

class AES : public Provider {
public:
	AES(class GSECrypto * owner);
	virtual ~AES();

	virtual bool doesSupport(const std::string &);
};

} /* namespace webworks */
#endif /* AES_HPP_ */
