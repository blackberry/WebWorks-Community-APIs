describe("in mock_environmnet", function () {
    describe("blackbery.loader", function () {
        beforeEach(function () {
            blackberry.Loader.clear("foo");
        });

        it("can return the funcitons passed into the loader", function () {
            var bar = function () {
                //place holder
                };
                
            blackberry.Loader.javascriptLoaded("foo", bar);
            expect(blackberry.Loader.getJavascript("foo")[0]).toBe(bar);
        });

        it("can register multiple function for a key", function () {
            var bar1 = function () {}, 
                bar2 = function () {}; 
            blackberry.Loader.javascriptLoaded("foo", bar1);
            blackberry.Loader.javascriptLoaded("foo", bar2);

            expect(blackberry.Loader.getJavascript("foo").length).toBe(2);
        });
    });

    describe("events", function () {
        it("throws an exception when calling registerEventHandler", function () {
            expect(blackberry.events.registerEventHandler).toThrow();
        });
    });

    describe("transport", function () {
        it("can create a RemoteFunctionCall", function () {
            expect(new blackberry.transport.RemoteFunctionCall()).toBeDefined();
        });

        it("exposes the url passed into the ctor", function () {
            expect(new blackberry.transport.RemoteFunctionCall("asdf")._url).toBe("asdf");
        });

    });
});
