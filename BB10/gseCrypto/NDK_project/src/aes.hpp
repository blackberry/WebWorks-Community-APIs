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

	virtual Json::Value generateKey(const std::string & alg, Json::Value & args);
};

class AESParams {
public:
	AESParams(class GSECrypto * owner);
	virtual ~AESParams();

	void create(int mode, size_t blockLen, bool withRandom);

private:
	class GSECrypto * owner;
	sb_Params params;

	friend class AESKey;
};

class AESKey {
public:
	AESKey(AESParams & owner);
	virtual ~AESKey();
	void generate(size_t size);
	void get(DataTracker & dt);

private:
	AESParams & owner;
	sb_Key key;
};

} /* namespace webworks */
#endif /* AES_HPP_ */
