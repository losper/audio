var util=require("util");
var a2sync=require("a2sync");
var pack=require("pack");
module.exports=function(c,runable){
    var cli=c;
    var t;
    var pos=new pack.outputStream();
    var pis=new pack.inputStream();
    pos.on("data",function(data){
	if(t && t.resume(data)){
	    t=undefined;
	}
    });
    pis.on("data",function(data){
	c.write(data);
    });
    c.on("data",function(data){
	pos.push(data);
    });
    this.json=function(){
	var msg=a2sync.yield(false);
	return msg;
    }
    this.play=function(path){
	var str={act:"play",path:path}
	pis.push(str);
	while(1){
	    if("play"==this.json().act){
		break;
	    }
	}
    }
    this.record=function(path,sec){
	var str={act:"record",path:path,sec:sec}
	pis.push(str);
	while(1){
	    if("record"==this.json().act){
		break;
	    }
	}
    }
    function run(rb){
	runable(rb);
	return true;
    }
    this.start=function(){
	t=new a2sync(run);
	t.resume(this);
    }
}
