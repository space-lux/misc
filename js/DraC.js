var DraC=(function(){
	var self={};
	self.$=function(i) {
		return document.getElementById(i);
	};
	self.max=function(a) {
		return Math.max.apply(null,a);
	};
	self.min=function(a) {
		return Math.min.apply(null,a);
	};
	
	self.createColorSet=function(number) {
		var	colors=[],
				step=768/number;
		for(var i=0;i<number;i++) {
			colors[i]=self.getColor(i*step);
		}
		return colors;
	};
	self.getColor=function(tone) { // tone in [0,768[
		var colorBase=512;
		var coeff=2;
		
		redTone=tone%768-384;
		greenTone=(tone+256)%768-384;
		blueTone=(tone+512)%768-384;
		
		var r=[];
		r.r=Math.round(Math.max(0,Math.min(256,(colorBase-coeff*Math.abs(redTone)))));
		r.g=Math.round(Math.max(0,Math.min(256,(colorBase-coeff*Math.abs(greenTone)))));
		r.b=Math.round(Math.max(0,Math.min(256,(colorBase-coeff*Math.abs(blueTone)))));
		
		return r;
	};
	
	
	self.pie=function(c,d,l) {
		var	w=c.width,
				h=c.height,
				t=0,
				ca,
				la=0;
				s=Math.min(h/2,w/2);
		var colors=self.createColorSet(d.length);
		c=c.getContext("2d");
		for(i in d) {
			t+=d[i].value;
			colors[i]='rgb('+colors[i].r+','+colors[i].g+','+colors[i].b+')';
			if(d[i].color)
				colors[i]=d[i].color;
			if(l) {
				if(!d[i].label)
					d[i].label="[no label]";
				var li=document.createElement("li");
				li.style.color=colors[i];
				li.appendChild(document.createTextNode(d[i].label));
				l.appendChild(li);
			}
		}
		for(i in d) {
			ca=2*Math.PI*d[i].value/t;
			c.beginPath();
			c.fillStyle=colors[i];
			c.arc(w/2,h/2,s,la,la+ca);
			c.lineTo(w/2,h/2);
			c.fill();
			la+=ca;
		}
	};
	
	self.line=function(c,d) {
		var	w=c.width,
				h=c.height,
				maxi=0,
				mini=0,
				n=d.values[0].length,
				hdiv=w/n,
				hstart=hdiv/2,
				fontsize=10,
				lineweight=4,
				vdiv,
				vstart=lineweight/2,
				vs=h-fontsize-lineweight,
				hs=(n-1)*hdiv,
				vtext=h-fontsize,
				hpos=hstart,
				vpos,
				vlinespacing=50,
				colors=self.createColorSet(d.values.length);
		function valueToY(v) {
			return vstart-v*vdiv+maxi*vdiv;
		}
		
		c=c.getContext("2d");
		c.font=fontsize+"px Arial";
		c.fillStyle="black";
		c.strokeStyle="rgb(175,175,255)";
		for(i in d.values) {
			maxi=Math.max(maxi,self.max(d.values[i]));
			mini=Math.min(mini,self.min(d.values[i]));
		}
		maxi*=1.1;
		mini*=1.1;
		
		vdiv=vs/(maxi-mini);
		vlinespacing=Math.pow(5,Math.floor(Math.log(vlinespacing/vdiv)/Math.log(5)));
		c.textAlign="center";
		for(i in d.values[0]) {	
			c.beginPath();
			c.moveTo(hpos,vstart);
			c.lineTo(hpos,vstart+vs);
			c.stroke();
			c.fillText(d.labels[i],hpos,2*vstart+vs+fontsize);
			hpos+=hdiv;
		}
		
		vpos=vs+(mini%vlinespacing)*vdiv;
		
		c.textAlign="end";
		while(vpos>0) {
				c.beginPath();
				c.moveTo(hstart,vpos+vstart);
				c.lineTo(hpos-hdiv,vpos+vstart);
				c.stroke();
				c.fillText(Math.round(maxi-(vpos)/vdiv),hstart-5,vpos+vstart+fontsize/2);
				vpos-=vlinespacing*vdiv;
		}
		
		for(i in d.values) {
			hpos=hstart;
			c.strokeStyle='rgb('+colors[i].r+','+colors[i].g+','+colors[i].b+')';
			if(d.colors && d.colors[i])
				c.strokeStyle=d.colors[i];
			for(j=0;j<d.values[i].length-1;j++) {
				c.beginPath();
				c.moveTo(hpos,valueToY(d.values[i][j]));
				hpos+=hdiv;
				c.lineTo(hpos,valueToY(d.values[i][j+1]));
				c.stroke();
			}
		}
	};
	
	return self;
})();
