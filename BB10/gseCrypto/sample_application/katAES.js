function katAES() {
	var plainText = katAESCBCvector.plainText;
	var vectors = katAESCBCvector.vectors;
	var message = "<table><tbody>";
	for ( var i = 0; i < vectors.length; ++i) {
		var vector = vectors[i];
		var params = {
			alg : "aes",
			mode : "cbc",
			key : {
				hex : vector.key
			},
			iv : {
				hex : vector.iv
			},
			input : {
				hex : vector.result
			}
		};
		var result = community.gseCrypto.decrypt(params);
		if (result.error) {
			alert(result.error);
		} else {
			var match = result.output.hex == plainText;
			var matchText = match ? "pass" : "fail";
			message += "<tr><td class='";
			message += matchText;
			message += "'>";
			message += matchText;
			message += "</td><td>";
			message += result.output.hex;
			message += "</td></tr>";
		}
	}
	log(message);
}

var katAESCBCvector = {
	"plainText" : "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f",
	"vectors" : [
			{
				"key" : "9518ce2135a3c179fde8914b7dcbf761",
				"iv" : "869c9c8ac208d5d469fdb1f409c91790",
				"result" : "ebec5e37d9992d75ffbf586365c2d57105e919adf773eb4e5458c4d251dc7000b5c40158eea2e1e4cc1ce265ee8dcff5e7bf020b0434fc42fee16bbb648ee02d"
			},
			{
				"key" : "02dfeaa8fe890d7f1db2639d4ea18666",
				"iv" : "4d7af01fccc07425e7f9fbc9cf64eecd",
				"result" : "45573d7c7717b6b6838375cce77761c069f99f471b5ee174ceb4711b6d83f7783f6c7bf68762a708a5084f9d84a03918b3f78c7dc4accd78913f200e1ba6188d"
			},
			{
				"key" : "1448cd8a5e4cd0b355aa86e08de29628",
				"iv" : "4f4dcd5f3cc06eaef70d1095cffaeda8",
				"result" : "b652d54ff1e3e6d9f194bcc04348a4f6fa5cc0fbb036d35bc7026841ac168ef76bbf04ca8ca8470c9f41949af1448e67ac6708eb7bcd4680c95b26298b0e74e9"
			},
			{
				"key" : "76c1431fb5ce95659def90c48d3674d1",
				"iv" : "cf9e0965fde3584ab72159c264289937",
				"result" : "03af4f74456f9056922f28d3d1be5160a40db0f409a2de8d18fde44144be858aad84c107b9db73a4db1d552e9d7cc276ad6085d5946c2c79cfbab21bd45907ee"
			},
			{
				"key" : "d06b229ebcfccf95f8f99f3f6382bc78",
				"iv" : "5d2dd3116fb5b868451ad395e2d4fa7d",
				"result" : "10adb5de507fb069b2afb6abd312c34a64f8d231454c4f249ab5e306ed3aba0a7118f2e7ff5e87edf3dbef0e7c027bca8ff6dc125349082a86f25ac6324575ea"
			},
			{
				"key" : "29966465d2797e896b85ba9ca4c2b8d7f35c1d66e61bc9a0",
				"iv" : "6c5033794a063c76edabf6d1e1b4b452",
				"result" : "7ec0e2289a4aaa05bcb1f0e4a39f70b43e815f41d3af672ae459f4e628b613f08da86b5d7dcb3fa04420802642d5a5772becb6696fcee2728af34007af6ffeb5"
			},
			{
				"key" : "6499d391e1c6a454c701180180e396d2c5b60ee937cb5e40",
				"iv" : "46fd66e685db0cf70aea350a9ac7d86a",
				"result" : "945c11b71d1c2b19b231067e91ee3dc0e86e0e8f763faba9b3b8f121686ca93c274d926544c3c4f8e001d4df9ac5d5bfa6de579f1504dc57e436bfbc005db7ae"
			},
			{
				"key" : "c19e594ffac09d28e1737e7a9e2a0703279752d6296c1d1d",
				"iv" : "12dd8077d210065ec701d8240d02db67",
				"result" : "157dd32d95c2153cc420d6eaf18afd48ea28551996bbbe2c5bb4a110b50fbb5410f79e88b3bddf3c14cbed63f9aadc1228072242afc6dad8318bd6a7c171addb"
			},
			{
				"key" : "4e501bfa26ff29863b9eeb15f32e78020811e38f72b17075",
				"iv" : "18a3f22124a602d236defa4960b0fcb4",
				"result" : "a6e0e43bc3b95d9aba5bc7a089a6a4d1cf2e2029c72fd52e584e327cceb9f134eaefc855185f1349037ae1ce1f782c1f29fdb14841ee3fe51a17489ce91764db"
			},
			{
				"key" : "284ba80c15492d1cd9c9a0e4b285d19ad204f49b19e1c827",
				"iv" : "dc8953990491476f4ce05138f5e77f2a",
				"result" : "9ba81c98595f4c97438f5c3d39acc4b1e2878408f20e2dc70b5c970792cfda14e23649d97a54fcb1939e30f107be93b4ed27e274181b3783da747059dc28a43f"
			},
			{
				"key" : "f446e1878f2dff0af27916add9935f531851519d27731b16c795ccc751e73c61",
				"iv" : "1d51bf010fc29aa4ca3eccdbd9dbcf44",
				"result" : "509bb807afefc51e2af50577493c7a3c6c8c5b92bff6db48bd051f4506906e0e88c811e4839fb34f9cc90a072465294239ac69c409220f77e0ad9b05e22d57e7"
			},
			{
				"key" : "1cd4ae016eceea930b3314f88d71077da4e2869eb155ca12ef8c89390a29ec44",
				"iv" : "b039de66c16cca9c4d1bd26f23dd101e",
				"result" : "386c00c51232c06231c462cec757fbb284289da6b6c814c4c6950f098597cce3b3e7f76ce7eabe6a01edf6d099de917b1e4d1b9f539fa943c41b413213612aa3"
			},
			{
				"key" : "3aa3bd6b0597950090188a7d0c8833870c37a69c4d56e9534c0d98913345a703",
				"iv" : "61ee4ee0e0c0f9a574cb78d051d4d89a",
				"result" : "576a904b74ba7ab01ed053b08dc28e51450a487508b3c4f0c94364ef259aa7e3fc08aff2b7dace6137a22f347a8a022b6505362bc4394874fe9ea4e0d39b96ed"
			},
			{
				"key" : "65fffc0633bfa6a3e507f90314c3a570b0ceab88046aef966991c29761d37188",
				"iv" : "78caee1ecab806e257412e4e9e6e47b6",
				"result" : "06c9c976addfc4e1e1e5ce9ef7d16fa3271e31c918ae8e8f5e98bcfc140831ddc9dbdbc4e95cb747ed11c6db90b3566d8d9498bc6681a49c9e834c040d6d9b87"
			},
			{
				"key" : "b9b1ead8f352870d3e91ae0ec503794d81ca5ae763ca8b37bcc455b702a3b17e",
				"iv" : "25441dd7f811af4be219627046db210a",
				"result" : "3d792734ba043c93ae56c8de69f070521604a60c1cbc426088523198325c780efc312ed4b74a456108c91f60511b518ee8d6a65c74e6516923096a4c21eb0105"
			} ]
};
