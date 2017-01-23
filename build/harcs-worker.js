var that = this;

var Module = {
  'print': function report(str) {
    that.postMessage(str);
  }
};

importScripts('harcs.js');


var cmd = Module.cwrap('cmd', 'void', ['string']);
this.addEventListener('message', function(e) {
  cmd(e.data)
}, false);