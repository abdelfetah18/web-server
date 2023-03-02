var registered_paths = ["/users/:id/admin/:username/another","/dashboard"];

var path = "/users/user/admin/test/another/";

for(let p of registered_paths){
	console.log(another_match(path,p));
}

function another_match(a,b){
	let i = 0;
	let j = 0;
	var querys = [];
	while(i < a.length && j < b.length){
		let a_1 = "";
		let b_1 = "";
		if(a[i] == "/"){
			i += 1;
			while(i < a.length && a[i] != '/'){
				a_1 += a[i];
				i += 1;
			}
		}

		if(b[j] == "/"){
			j += 1;
			while(j < b.length && b[j] != '/'){
				b_1 += b[j];
				j += 1;
			}
		}

		if(!b_1.startsWith(":")){
			if(a_1 != b_1){
				return { is_equal: false, querys };
			}
		}else{
			querys.push([b_1,a_1]);
		}
	}
	if((a.length - i) == 0 && (b.length - j) == 0 ){
		return { is_equal: true, querys };
	}else{
		if(a.length - i == 1 && a[i] == "/"){
			return { is_equal: true, querys };
		}
		return { is_equal: false, querys };
	}
}