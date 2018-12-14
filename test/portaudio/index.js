var net=require("net");
var util=require("util");
var pack=require("pack");
var fs=require("fs");
var Robot=require("./src/robot");
var cli=require("./client.js");
function runTest(rb){
    rb.record("test.pcm",6);
    rb.play("test.pcm");
    // rb.click("com.android.dialer:id/floating_action_button");
    // rb.wait("com.android.dialer:id/dialpad_floating_action_button);
}
var s=net.createServer(function(c){
    // var pos=new pack.outputStream();
    // pos.on("data",function(msg){
    // 	console.log(msg);
    // });
    // c.on("data",function(data){
    // 	pos.push(data);
    // })
    var inst=new Robot(c,runTest);
    inst.start();
});

s.listen(8009);
cli.test();
