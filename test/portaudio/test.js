const pcm=require("./pcm.js");
pcm.record("test.pcm",3,function(){
    pcm.play("test.pcm",function(){
	console.log("play done in test.js");
    });
});
