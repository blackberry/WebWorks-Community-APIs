function katSHA() {
	var which = Array("raw", "hex", "b64");

	var testVectors = katSHAvector;

	var message = "";

	message += "<h2>SHA KAT</h2>";
	message += "<table>";

	for ( var i = 0; i < testVectors.length; ++i) {
		var suite = testVectors[i];
		for ( var j = 0; j < which.length; ++j) {
			var encoding = which[j];
			var toHash = suite[encoding];

			var params = Object();
			params[encoding] = toHash;

			var results = suite.results;
			for ( var k = 0; k < results.length; ++k) {
				var kat = results[k];
				var alg = kat.alg;
				params.alg = alg;
				
				var live = community.gseCrypto.hash(params);

				var match = kat.hex == live.hex;
				message += ("<tr>");
				message += ("<td class='" + (match ? "pass" : "fail") + "'>"
						+ match + "</td>");

				match = kat.b64 == live.b64;
				message += ("<td class='" + (match ? "pass" : "fail") + "'>"
						+ match + "</td>");

				message += ("<td>" + JSON.stringify(params) + "</td>");

				message += ("<td>" + kat.hex + "<br />" + live.hex + "</td>");
				message += ("<td>" + kat.b64 + "<br />" + live.b64 + "</td>");

				message += ("</tr>");
			}
		}
	}
	message += ("</table>");

	log(message);
}

var katSHAvector = [
		{
			"raw" : "",
			"hex" : "",
			"b64" : "",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "da39a3ee5e6b4b0d3255bfef95601890afd80709",
						"b64" : "2jmj7l5rSw0yVb/vlWAYkK/YBwk="
					},
					{
						"alg" : "SHA-256",
						"hex" : "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
						"b64" : "47DEQpj8HBSa+/TImW+5JCeuQeRkm5NMpJWZG3hSuFU="
					},
					{
						"alg" : "SHA-384",
						"hex" : "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b",
						"b64" : "OLBgp1GsljhM2TJ+sbHjaiH9txEUvgdDTAzHv2P24donTt6/529l+9Ua0vFImLlb"
					},
					{
						"alg" : "SHA-512",
						"hex" : "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e",
						"b64" : "z4PhNX7vuL3xVChQ1m2AB9Yg5AULVxXcg/SpIdNs6c5H0NE8XYXysP+DGNKHfuwvY7kxvUdBeoGlODJ6+SfaPg=="
					} ]
		},
		{
			"raw" : "M",
			"hex" : "4d",
			"b64" : "TQ==",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "c63ae6dd4fc9f9dda66970e827d13f7c73fe841c",
						"b64" : "xjrm3U/J+d2maXDoJ9E/fHP+hBw="
					},
					{
						"alg" : "SHA-256",
						"hex" : "08f271887ce94707da822d5263bae19d5519cb3614e0daedc4c7ce5dab7473f1",
						"b64" : "CPJxiHzpRwfagi1SY7rhnVUZyzYU4NrtxMfOXat0c/E="
					},
					{
						"alg" : "SHA-384",
						"hex" : "481c2cb6a1dafdcc76e99d90d1705be284a772836d86015966b171afaae28424bf05b75aa20bb444a145351df9b7ac30",
						"b64" : "SBwstqHa/cx26Z2Q0XBb4oSncoNthgFZZrFxr6rihCS/Bbdaogu0RKFFNR35t6ww"
					},
					{
						"alg" : "SHA-512",
						"hex" : "253405e03b91441a6dd354a9b72e040068b1bfe10e83eb1a64a086c05525d8ccae2bf09130c624af50d55c3522a4fbb7c18cfc8dd843e5f4801d9ad2b5164b12",
						"b64" : "JTQF4DuRRBpt01Spty4EAGixv+EOg+saZKCGwFUl2MyuK/CRMMYkr1DVXDUipPu3wYz8jdhD5fSAHZrStRZLEg=="
					} ]
		},
		{
			"raw" : "Mo",
			"hex" : "4d6f",
			"b64" : "TW8=",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "91e885d8e5afb3adf939ae0b29774a7cdd738ce9",
						"b64" : "keiF2OWvs635Oa4LKXdKfN1zjOk="
					},
					{
						"alg" : "SHA-256",
						"hex" : "d23e867e38e89ebf17d684893be20aa36972233e45a845b4345a2147951aff93",
						"b64" : "0j6Gfjjonr8X1oSJO+IKo2lyIz5FqEW0NFohR5Ua/5M="
					},
					{
						"alg" : "SHA-384",
						"hex" : "5afcf7bafd36966e6913f4ffdbcb3f225a7462dd13be4b262fc4d079e1bf6548d700a277aa70398c5c9a9bb779900807",
						"b64" : "Wvz3uv02lm5pE/T/28s/Ilp0Yt0TvksmL8TQeeG/ZUjXAKJ3qnA5jFyam7d5kAgH"
					},
					{
						"alg" : "SHA-512",
						"hex" : "10893a12a15964dde45d8fe1845e84ccb5303b304e89034bf54e496ba7e660f45286ff46eb3a33cd8aa2c404dc88b1f42efb2b07611ab316b0963d4fb93ea348",
						"b64" : "EIk6EqFZZN3kXY/hhF6EzLUwOzBOiQNL9U5Ja6fmYPRShv9G6zozzYqixATciLH0LvsrB2Easxawlj1PuT6jSA=="
					} ]
		},
		{
			"raw" : "Mon",
			"hex" : "4d6f6e",
			"b64" : "TW9u",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "24b2a0993d0cfa93c44282d6bba72bcf58b300d6",
						"b64" : "JLKgmT0M+pPEQoLWu6crz1izANY="
					},
					{
						"alg" : "SHA-256",
						"hex" : "f40d7f51f69edfaffa29c42910fbc6af6a822f1279162d486b4a7e11c3e0ae9b",
						"b64" : "9A1/Ufae36/6KcQpEPvGr2qCLxJ5Fi1Ia0p+EcPgrps="
					},
					{
						"alg" : "SHA-384",
						"hex" : "9d4af75654983ae6543e232a6846a5a4746f1c3e72850b15d6b5f4000cd0e453230bfd725165b595227ca14d3870c28d",
						"b64" : "nUr3VlSYOuZUPiMqaEalpHRvHD5yhQsV1rX0AAzQ5FMjC/1yUWW1lSJ8oU04cMKN"
					},
					{
						"alg" : "SHA-512",
						"hex" : "4fc2591de8b4e95f805a7e9f8332492bcf2196fa42326f2c859f75297fdd98af73d8760886c9688474f305992f4625dda5811c6e0f22b03ec273aa38a209b9bb",
						"b64" : "T8JZHei06V+AWn6fgzJJK88hlvpCMm8shZ91KX/dmK9z2HYIhslohHTzBZkvRiXdpYEcbg8isD7Cc6o4ogm5uw=="
					} ]
		},
		{
			"raw" : "Mont",
			"hex" : "4d6f6e74",
			"b64" : "TW9udA==",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "3f6e2a950a100e8375cf242cbb0001a039ae9968",
						"b64" : "P24qlQoQDoN1zyQsuwABoDmumWg="
					},
					{
						"alg" : "SHA-256",
						"hex" : "335b04cee4f46422b5fe7ec552dc18651e7825358512e698bcd1012369202936",
						"b64" : "M1sEzuT0ZCK1/n7FUtwYZR54JTWFEuaYvNEBI2kgKTY="
					},
					{
						"alg" : "SHA-384",
						"hex" : "7afa715207159465603c1c061ec47e2aaa992b60faa5b1a61774c69efda32f812d7dfa25aacd9c90fd4252d8f4f639ec",
						"b64" : "evpxUgcVlGVgPBwGHsR+KqqZK2D6pbGmF3TGnv2jL4Etffolqs2ckP1CUtj09jns"
					},
					{
						"alg" : "SHA-512",
						"hex" : "4e4c62efded4dbff38e488d06ddf632257444f91b337333d8302e6efb003201bd28dbe4426289e99912229ee4fa6a141f943959c93cef7645ec1b64df3a981a8",
						"b64" : "Tkxi797U2/845IjQbd9jIldET5GzNzM9gwLm77ADIBvSjb5EJiiemZEiKe5PpqFB+UOVnJPO92RewbZN86mBqA=="
					} ]
		},
		{
			"raw" : "Montr",
			"hex" : "4d6f6e7472",
			"b64" : "TW9udHI=",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "68c4ccd6dafa45367a232b3109aa9a9c56e0915a",
						"b64" : "aMTM1tr6RTZ6IysxCaqanFbgkVo="
					},
					{
						"alg" : "SHA-256",
						"hex" : "4073843182a0373cf8d34a1d6afbca0a5f77e4048f085c33dc8ae4e5d547fa71",
						"b64" : "QHOEMYKgNzz400odavvKCl935ASPCFwz3Irk5dVH+nE="
					},
					{
						"alg" : "SHA-384",
						"hex" : "1f21743f8d46489ee3a24f8b4322b61070da57481f673ad62369181d63b41a4f81776d99892cb89a82fae6c5b11b1912",
						"b64" : "HyF0P41GSJ7jok+LQyK2EHDaV0gfZzrWI2kYHWO0Gk+Bd22ZiSy4moL65sWxGxkS"
					},
					{
						"alg" : "SHA-512",
						"hex" : "4cbc98c3644c43341ab9fc8af30d48481b5bac1280cf4da8a9848e3c6426779067961081bf4a0c86c92096a1b8e765ceaa0b14b5656e6ebf79e1f70323ba9893",
						"b64" : "TLyYw2RMQzQaufyK8w1ISBtbrBKAz02oqYSOPGQmd5BnlhCBv0oMhskglqG452XOqgsUtWVubr954fcDI7qYkw=="
					} ]
		},
		{
			"raw" : "Montr\u00e9",
			"hex" : "4d6f6e7472c3a9",
			"b64" : "TW9udHLDqQ==",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "155a5b56741e18105ba761f826a79800069f5fda",
						"b64" : "FVpbVnQeGBBbp2H4JqeYAAafX9o="
					},
					{
						"alg" : "SHA-256",
						"hex" : "56d0578d661355ccc0f3ac6d3a4e7cde7775fea47c7ebf7a31f2cb65fc865913",
						"b64" : "VtBXjWYTVczA86xtOk583nd1/qR8fr96MfLLZfyGWRM="
					},
					{
						"alg" : "SHA-384",
						"hex" : "113270b98bd486df4ec2305c61e40b2e608dbf57226d1d4a3331b5a2ed9bf895a68b38a9933bad56099bfdbd59acf55a",
						"b64" : "ETJwuYvUht9OwjBcYeQLLmCNv1cibR1KMzG1ou2b+JWmizipkzutVgmb/b1ZrPVa"
					},
					{
						"alg" : "SHA-512",
						"hex" : "e34663995db4dfc076768b2c3c449a071b9d30cd9c4717b505a3d367766e79c532d14f41d95d205ecdb5006e4cb0ef0f5cd49635ba508adf1602eed2eda23d66",
						"b64" : "40ZjmV2038B2dossPESaBxudMM2cRxe1BaPTZ3ZuecUy0U9B2V0gXs21AG5MsO8PXNSWNbpQit8WAu7S7aI9Zg=="
					} ]
		},
		{
			"raw" : "Montr\u00e9a",
			"hex" : "4d6f6e7472c3a961",
			"b64" : "TW9udHLDqWE=",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "0059c4f9ac2aadea5a60c9f61b321d3f50d69e74",
						"b64" : "AFnE+awqrepaYMn2GzIdP1DWnnQ="
					},
					{
						"alg" : "SHA-256",
						"hex" : "f01cf89b990c6da4d58388194d6d385ea5b6195a23f3cbe6c674681e080c782e",
						"b64" : "8Bz4m5kMbaTVg4gZTW04XqW2GVoj88vmxnRoHggMeC4="
					},
					{
						"alg" : "SHA-384",
						"hex" : "7e49e0ed776f802626113bd70d5e88af95986d4a096859cfd5d6f562dd1bad465ad0efdd99d1295bafdf228f394ef11e",
						"b64" : "fkng7XdvgCYmETvXDV6Ir5WYbUoJaFnP1db1Yt0brUZa0O/dmdEpW6/fIo85TvEe"
					},
					{
						"alg" : "SHA-512",
						"hex" : "3dd64123a9a8a463dd5498ffd167e2dcde52a96f7c4fa888b2222ba6637ca2386eeb35aa7c6131655bc60c3444536678142fcecd06999c906b1934d2fc6220fd",
						"b64" : "PdZBI6mopGPdVJj/0Wfi3N5SqW98T6iIsiIrpmN8ojhu6zWqfGExZVvGDDREU2Z4FC/OzQaZnJBrGTTS/GIg/Q=="
					} ]
		},
		{
			"raw" : "Montr\u00e9al",
			"hex" : "4d6f6e7472c3a9616c",
			"b64" : "TW9udHLDqWFs",
			"results" : [
					{
						"alg" : "SHA-1",
						"hex" : "de5713dcb9dd534e0bdf1f7ee8578e2457f61fdc",
						"b64" : "3lcT3LndU04L3x9+6FeOJFf2H9w="
					},
					{
						"alg" : "SHA-256",
						"hex" : "bcbbb762b764a435b7e65cbde8489730441194ff16ef587a0823aa3cc2862b38",
						"b64" : "vLu3YrdkpDW35ly96EiXMEQRlP8W71h6CCOqPMKGKzg="
					},
					{
						"alg" : "SHA-384",
						"hex" : "7f009327f21b8c9a0e38937448cb028c2fcfca406c651427a925cf4c32528a1cd64dc4a22a25543af9cbabfcc4802f88",
						"b64" : "fwCTJ/IbjJoOOJN0SMsCjC/PykBsZRQnqSXPTDJSihzWTcSiKiVUOvnLq/zEgC+I"
					},
					{
						"alg" : "SHA-512",
						"hex" : "c3f54a4ffb8f2c24beb03c5608b14836ecfae8097dd020a5aed0d5186c1c5abc74b807974d74166b7551fbc5ec99cf15bec634872337d5cdddc38f0e1639d6ac",
						"b64" : "w/VKT/uPLCS+sDxWCLFINuz66Al90CClrtDVGGwcWrx0uAeXTXQWa3VR+8Xsmc8VvsY0hyM31c3dw48OFjnWrA=="
					} ]
		} ];
