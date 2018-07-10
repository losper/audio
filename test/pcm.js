const audio=require("audio");
const opt={channels:2,format:8,rate:44100,frames:44100};
const fs=require("fs");

function play(path,cb){
    var rs=new fs.ReadStream(path);
    var aos=new audio.OutputStream(opt);
    var written=0;
    var buf;
    aos.on("drain",function(){
	written=aos.push(buf);
	if(buf.length>written){
	    buf=buf.subarray(written);
	}else{
	    rs.resume();
	}
    })
    aos.on("done",function(){
	cb();
    })
    rs.on("data",function(data){
	written=aos.push(data);
	console.log("rs.on",data.length,written,data.length>written);
	if(data.length>written){
    	    console.log("pause");
    	    rs.pause();
    	    buf=data.subarray(written);
	}
    });
    rs.on("end",function(){
	console.log("rs.end");
	rs.close();
	aos.shutdown();
    })
}
function record(path,sec,cb){
    var ais=new audio.InputStream(opt);
    var gsize=0;
    ais.on("data",function(data){
	if(gsize){
    	    fs.appendFile("test.pcm",data);
	}else{
	    gsize=1;
	    console.log("rewrite!!!!");
    	    fs.writeFile("test.pcm",data);
	}
    });
    ais.on("done",function(){
	ais.shutdown();
	console.log("record done");
	cb();
    })
    ais.record(sec);
}
module.exports={
    play:play,
    record:record
};
