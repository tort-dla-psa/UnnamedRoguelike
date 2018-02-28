#include "material.h"
#include "tile.h"
#include "item.h"
#include<fstream>
#include<vector>
#include<ncurses.h>
namespace FileOperations{
	static tileenums::DepthType stoenum(std::string str){
		if(str=="cover")	return tileenums::cover;
		if(str=="surface")	return tileenums::surface;
		if(str=="middle")	return tileenums::middle;
		if(str=="deep")		return tileenums::deep;
		return tileenums::none;
	}

	static std::vector<item*> LoadOres(std::vector<material*> materials){
		std::vector<item*> ores;
		std::ifstream oresfile("raws/items/ores/ores.raw");
		std::string temptype="none";
		std::string val="none";
		std::string name="none";
		char img='@';
		material* mat = NULL;
		for(std::string line; getline(oresfile,line);){
			if(line=="["){
				continue;
			}
			if(line=="]"){
				ores.push_back(new item(name,img,mat));
				img='@';
				mat=NULL;
				temptype="none";
				val="none";
				name="none";
				continue;
			}
			short pos = line.find(":");
			temptype = line.substr(0,pos);
			val = line.substr(pos+1);
			if(temptype=="img"){
				img=val[0];
			}else if(temptype=="material"){
				for (auto &element:materials){
					if(element->GetName()==val)
						mat=element;
				}
			}else if(temptype=="name"){
				name=val;
			}
		}
		oresfile.close();
		return ores;
	}

	static std::vector<tile*> LoadTiles(std::vector<item*> ores){
		std::vector<tile*> tiles;
		std::ifstream tilesfile("raws/tiles/tiles.raw");
		std::string temptype="none";
		std::string val="none";
		std::string orename="none";
		std::string name = "none";
		tileenums::DepthType depth= tileenums::DepthType::none;
		char img='@';
		item* ore = NULL;
		short dropchance=0;
		for(std::string line; getline(tilesfile,line);){
			if(line=="["){
				continue;
			}
			if(line=="]"){
				if(ore->GetMat()->GetPass()){
					tiles.push_back(new tilewspace(img,name,ore,depth));
				}else{
					tiles.push_back(new tile(img,name,dropchance,ore,depth));
				}
				img='@';
				name = "none";
				orename = "none";
				ore=NULL;
				dropchance=0;
				continue;
			}
			short pos = line.find(":");
			temptype = line.substr(0,pos);
			val = line.substr(pos+1);
			if(temptype=="img"){
				img=val[0];
			}else if(temptype=="name"){
				name=val;
			}else if(temptype=="depthtype"){
				depth=stoenum(val);
			}else if(temptype=="dropchance"){
				dropchance=std::stoi(val);
			}else if(temptype=="dropproduct"){
				orename=val;
				for(auto element:ores){
					if(element->GetName()==val)
						ore=element;
				}
			}
		}
		tilesfile.close();
		return tiles;
	}
	static std::vector<material*> LoadMaterials(){
		std::vector<material*> materials;
		std::ifstream stones("raws/materials/stones.raw");
		std::string name="";
		std::string type="";
		std::string temptype="";
		std::string val="";
		short melting=-1;
		short density=-1;
		for(std::string line; getline(stones,line);){
			if(line=="["){
				continue;
			}
			if(line=="]"){
				if(type=="gas"){
					materials.push_back(new gasMAT(density,name));
				}else{
					materials.push_back(new stoneMAT(density,melting,name));
				}
				name="generic";
				type="";
				melting=-1;
				density=-1;
				continue;
			}
			short pos = line.find(":");
			temptype = line.substr(0,pos);
			val = line.substr(pos+1);
			if(temptype=="name"){
				name=val;
			}else if(temptype=="type"){
				type=val;
			}else if(temptype=="melting"){
				melting=std::stoi(val);
			}else if(temptype=="density"){
				density=std::stoi(val);
			}
		}
		stones.close();
		return materials;
	}

};
