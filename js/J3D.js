var J3D=(function(){
	var jl={};
	
	
	jl.mulMat=function(mat1,mat2) {
		matR=[];
		for(var i in mat1) {
			matR[i]=[];
			for(var j in mat2[0]) {
				matR[i][j]=0;
			}
		}
		for(var i in matR) {
			for(var j in matR[i]) {
				for(var k in mat2) {
					matR[i][j]+=mat1[i][k]*mat2[k][j];
				}
			}
		}
		return matR;
	};
	
	jl.identity=function() {
		return [
			[1,0,0,0],
			[0,1,0,0],
			[0,0,1,0],
			[0,0,0,1]
		];
	};
	
	jl.translateMat=function(x,y,z) {
		return [
			[1,0,0,0],
			[0,1,0,0],
			[0,0,1,0],
			[x,y,z,1]
		];
	};
	
	jl.scaleMat=function(x,y,z) {
		return [
			[x,0,0,0],
			[0,y,0,0],
			[0,0,z,0],
			[0,0,0,1]
		];
	};
	
	jl.degToRad=function(deg) {return deg*Math.PI/180;};
	
	jl.rotateMatX=function(x) {
		return [
			[1,0,0,0],
			[0,Math.cos(x),Math.sin(x),0],
			[0,-Math.sin(x),Math.cos(x),0],
			[0,0,0,1]
		];
	};
	
	jl.rotateMatY=function(y) {
		return [
			[Math.cos(y),0,Math.sin(y),0],
			[0,1,0,0],
			[-Math.sin(y),0,Math.cos(y),0],
			[0,0,0,1]
		];
	};
	
	jl.rotateMatZ=function(z) {
		return [
			[Math.cos(z),Math.sin(z),0,0],
			[-Math.sin(z),Math.cos(z),0,0],
			[0,0,1,0],
			[0,0,0,1]
		];
	};
	
	jl.translate=function(v,x,y,z) {
		return jl.mulMat(v,jl.translateMat(x,y,z));
	};
	
	jl.scale=function(v,x,y,z) {
		return jl.mulMat(v,jl.scaleMat(x,y,z));
	};
	
	jl.rotateX=function(v,x) {
		return jl.mulMat(v,jl.rotateMatX(x));
	};
	
	jl.rotateY=function(v,y) {
		return jl.mulMat(v,jl.rotateMatY(y));
	};
	
	jl.rotateZ=function(v,z) {
		return jl.mulMat(v,jl.rotateMatZ(z));
	};
	
	jl.projection=function(v) {
		var vR=[];
		for(var i in v) {
			if(v[i][2]>0)
				vR[i]=[v[i][0]/v[i][2],v[i][1]/v[i][2],v[i][2]];
				//vR[i]=[v[i][0],v[i][1],v[i][2]];
			else
				vR[i]=[0,0,0];
		}
		return vR;
	};
	
	jl.createZB=function(x,y) {
		ZB=[];
		ZB.init=function(){
			for(var i=0;i<y;i++) {
				ZB[i]=[];
				for(var j=0;j<x;j++) {
					ZB[i][j]=100;
				}
			}
		}
		ZB.init();
		return ZB;
	};
	
	jl.drawPixelZB=function(x,y,z,ZB,drawCB,params) {
		params.x=x;
		params.y=y;
		//x=Math.round(x);
		//y=Math.round(y);
		//if(ZB[y][x]>z && z>0) {
		if(ZB[y][x]>z) {
			drawCB(params);
			ZB[y][x]=z;
		}
		return ZB;
	};
	
	/*jl.camera=function() {
		var cam={};
		
		var matrix;
		var FPYangle;
		var Zoom;
		
		cam.init=function() {
			matrix=jl.identity();
			FPYangle=0;
			Zoom=1;
		};
		
		cam.move=function(x,y,z) {
			matrix=jl.translate(matrix,-x,-y,-z);
		};
		
		cam.rotateX=function(x) {
			matrix=jl.rotateX(matrix,-x);
		};
		
		cam.rotateY=function(y) {
			matrix=jl.rotateY(matrix,-y);
		};
		
		cam.rotateZ=function(z) {
			matrix=jl.rotateZ(matrix,-z);
		};
		
		cam.rotate=function(x,y) {
			cam.rotateX(x);
			FPYangle+=y;
		};
		
		cam.getMatrix=function() {
			return jl.scale(matrix,Zoom,Zoom,1);
		};
		
		cam.FPMatrix=function() {
			return jl.scale(jl.rotateY(matrix,-FPYangle),Zoom,Zoom,1);
		};
		
		cam.projection=function(v) {
			return jl.projection(jl.mulMat(v,cam.getMatrix()));
		};
		
		cam.FPprojection=function(v) {
			return jl.mulMat(v,cam.FPMatrix());
		};
		
		cam.zoom=function(factor) {
			Zoom*=factor;
		};
		
		cam.setZoom=function(z) {
			Zoom=z;
		};
		
		cam.init();

		return cam;
	};*/
	
	return jl;
})();
