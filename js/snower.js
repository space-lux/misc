var SNOWER=(function(){
	var snower={};
	
	function r(min, max) {
		var nb = min + (max-min+1)*Math.random();
		return Math.floor(nb);
	}
	
	var flakes=[];
	for(var i=0;i<150;i++) {
		flakes[i]=document.createElement("img");
		flakes[i].style.position="fixed";
		flakes[i].s=r(1,5);
		flakes[i].style.width=flakes[i].s+"%";
		flakes[i].style.zIndex=flakes[i].s+1000;
		flakes[i].xp=r(-1,99);
		flakes[i].yp=r(-5,100);
		flakes[i].style.left=flakes[i].xp+"%";
	}
	
	function snow() {
		for(i in flakes) {
			flakes[i].yp=1*flakes[i].yp+0.1*flakes[i].s;
			if(flakes[i].yp>100) {
				flakes[i].yp= -10;
				flakes[i].style.top=flakes[i].yp+"%";
				flakes[i].s=r(1,5);
				flakes[i].style.zIndex=flakes[i].s+1000;
				flakes[i].style.width=flakes[i].s+"%";
			}
			flakes[i].style.top=flakes[i].yp+"%";
			
		}
		setTimeout(snow,30);
	}
	
	snower.start=function(src){
		for(i in flakes) {
			document.body.appendChild(flakes[i]);
			flakes[i].src=src;
		}
		snow();
	}
	
	return snower;
})();
