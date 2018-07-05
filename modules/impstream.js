const Stream=require("stream");
const audio=require("./audio.passoa")
function InputStream(opt){
    InputStream.prototype.init.call(this);
    this.callback=function(ev,data){
	this.trigger(ev,data);
    }
    var inst=audio.open(opt.channels,opt.format,opt.rate,opt.frames,this.callback.bind(this));
    this.record=function(sec){
	audio.record(inst,sec);
    }
    this.destroy=function(){
	audio.destroy(inst);
    }
}
InputStream.prototype = new Stream();

function OutputStream(opt){
    OutputStream.prototype.init.call(this);
    this.callback=function(){
	this.trigger.apply(this,arguments);
    }
    var inst=audio.open(opt.channels,opt.format,opt.rate,opt.frames,this.callback.bind(this));
    
    this.on("data",function(data){
	console.log(inst,data);
	audio.play(inst,data);
    });
    this.destroy=function(){
	audio.destroy(inst);
    }
    
}
OutputStream.prototype = new Stream();


module.exports={
    InputStream:InputStream,
    OutputStream:OutputStream
}
