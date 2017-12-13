#include "material.h"
#include "tile.h"
#include<fstream>
#include<vector>
#include<ncurses.h>
namespace FileOperations{
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
				printw("%c ",img);refresh();
				ores.push_back(new item(name,img,50,0,mat));
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
		printw("\n");
		oresfile.close();
		return ores;
	}

	static std::vector<tiletype*> LoadTiles(std::vector<material*> materials,
						std::vector<item*> ores)
	{
		std::vector<tiletype*> tiles;
		std::ifstream tilesfile("raws/tiles/tiles.raw");
		std::string temptype="none";
		std::string val="none";
		std::string orename="none";
		char img='@';
		material* mat = NULL;
		item* ore = NULL;
		short dropchance=0;
		for(std::string line; getline(tilesfile,line);){
			if(line=="["){
				continue;
			}
			if(line=="]"){
				printw("%c ",img);refresh();
				tiles.push_back(new tiletype(img,dropchance,ore,mat));
				img='@';
				mat=NULL;
				ore=NULL;
				dropchance=0;
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
			}else if(temptype=="dropchance"){
				dropchance=std::stoi(val);
			}else if(temptype=="dropproduct"){
				orename=val;
				for(auto &element:ores){
					if(element->GetName()==val)
						ore=element;
				}
			}

		}
		printw("\n");
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
		short value=-1;
		short melting=-1;
		short density=-1;
		for(std::string line; getline(stones,line);){
			if(line=="["){
				continue;
			}
			if(line=="]"){
				printw("%s ",name.c_str());refresh();
				if(type=="gas"){
					materials.push_back(new gasMAT(density,value,name));
				}else{
					materials.push_back(new stoneMAT(density,value,melting,name));
				}
				name="";
				type="";
				value=-1;
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
			}else if(temptype=="value"){
				value=std::stoi(val);
			}else if(temptype=="melting"){
				melting=std::stoi(val);
			}else if(temptype=="density"){
				density=std::stoi(val);
			}
		}
		printw("\n");
		stones.close();
		return materials;
	}
};
