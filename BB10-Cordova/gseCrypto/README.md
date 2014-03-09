# GSE Crypto

This extension allows WebWorks code to call features of the cryptographic library included in BlackBerry 10.

Currently implemented algorithms include:

* Pseudo-Random Number Generator
* Cryptographic Hash Functions
    * MD5 (**not** recommended for cryptographic use)
    * SHA1 (**not** recommended for cryptographic use)
    * SHA2 (224/256/384/512 bits)
* Block Ciphers
    * AES-CBC 128/192/256

# Getting Started
## Warning
Use this cryptographic library at your own risk. While native functions are FIPS validated, it's _possible_ that this extension has introduced errors, unlikely as that may seem. In addition, if you plan to use any of these functions, make sure that you know what you are doing: the security of these algorithms means nothing if they are poorly used.

## Data

Cryptographic data is binary. You need to be clear on which bytes you want to work with. For this reason the extension accepts data as hexadecimal (hex) or Base64 (b64) encoding of the bytes.

Hexadecimal is easier to work with since concatenating two values is as simple as concatenating the strings. Eg:

```javascript
{
    "input" : {
        "hex" : "b0b5cafeb0b5cafeb0b5cafe"
    }
}
```

Base64 is the preferred encoding of MIME. Some features of the Invocation Framework expect b64 encoded data. Data URIs have their b64 encoded. Eg:

```javascript
{
    "input" : {
        "b64" : "sLXK/rC1yv6wtcr+"
    }
}
```

Binary data coming back from the extension is available as in both encodings: hex and b64.

## Errors

Errors will be given as a member of the result object. Eg:

```javascript
{
    "error" : "This is the error message"
}
```

# Operations
Current operations you can call are:

* random
* hash
* encrypt
* decrypt

The native API can be found [here](http://developer.blackberry.com/native/reference/core/com.qnx.doc.crypto.lib_ref/topic/manual/intro.html). The library contains a plethora of functions, and if there is something not yet implemented here that you would like to see, please feel free to make a request by commenting on this repository.

## random

random expects an object containing only size, set to an integer (not a string), which is the number of bytes (not bits) of random data that is desired. Eg:

```javascript
{
    "size" : 16
};
```

A response will have the output attribute set to the random data. Eg:
```javascript
{
    "output" : {
        "b64" : "Tn7tXLqtt3BNPf6QxyRZZg==",
        "hex" : "4e7eed5cbaadb7704d3dfe90c7245966"
    }
}

## hash

hash expects an object with algorithm (alg) and input data (input) set.

Algorithm options are: "md5", "sha1", "sha224", "sha256", "sha384" or "sha512"

Input can be hex, b64 (or both but only the hex will be used). Eg:

```javascript
{
    "alg" : "sha256",
    "input" : {
        "hex" : "0123abcd"
    }
};
```

The (non-error) output is an object with an attribute called output. Output itself is an object containing a hex and b64 encoding of the output. Eg:
```javascript
{
    "output" : {
        "b64":"ZOqwcFOUUBztD/mRv2kHf9OEbB2WTj2yjZYAiRcV2Eg=",
        "hex":"64eab0705394501ced0ff991bf69077fd3846c1d964e3db28d9600891715d848"
    }
}
```

## encrypt
Encrypt expects an object with algorithm (alg) set. Each individual encryption implementation may expect more values to be set.
**Currently only the AES algorithm is implemented**

### encrypt – AES
Encrypt – AES expects the following values
* Algorithm (alg) 
* Mode (mode)					(only CBC is currently implemented)
* Key (key)						(128, 192, or 256 bits as hex or b64)
* Initialization Vector (iv)	(CBC requires a 128-bit IV, hex or b64)
* Data to encrypt (input)		(hex or b64);
Example:

```javascript
{
    "alg" : "aes",
    "mode" : "cbc",
    "key" : {
        "b64":"AAAAAAAAAAAAAAAAAAAAAA=="
    },
    "input" : {
        "b64":"EjSrzRI0q80SNKvNEjSrzQ=="
    },
    "iv" : {
        "b64":"AAAAAAAAAAAAAAAAAAAAAA=="
    }
}
```

If there are no errors, the output will be as follows:

```javascript
{
    "output" : {
         "b64":"Tn7tXLqtt3BNPf6QxyRZZg==",
         "hex":"4e7eed5cbaadb7704d3dfe90c7245966"
    }
}
```

## decrypt

Encrypt expects an object with algorithm (alg) set. Each individual encryption implementation may expect more values to be set.
**Currently only the AES algorithm is implemented**

### decrypt – AES

Decrypt – AES expects and returns the same attributes as encrypt – AES.

# Padding
Security Builder GSE does not implement padding algorithms for block ciphers. Algorithms thus expect input of the appropriate block size only, and an error will result if this is not the case.
There are many ways to pad data, but keep in mind that padding itself does not increase security. See [wikipedia](http://en.wikipedia.org/wiki/Padding_%28cryptography%29) for an overview of existing padding schemes.

# Usage Examples
See gseCrypto/sample/www/js/index.js for examples of all operations in use.

