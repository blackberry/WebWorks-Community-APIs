/*
 * SHA.hpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#ifndef SHA_HPP_
#define SHA_HPP_

#include "provider.hpp"

namespace gsecrypto {

class SHA : public Provider {
public:
	SHA(class GSECrypto &);
	virtual ~SHA();

	virtual bool doesSupport(const std::string &);
	virtual Json::Value hash(const std::string & algorithm, Json::Value &);

};

} /* namespace gsecrypto */
#endif /* SHA_HPP_ */
