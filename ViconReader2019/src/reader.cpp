/**
 * @brief Function connect to the new Vicon broadcaster for linux systems.
 * Parses messages and prints to screen.
 */
#include <iostream>
#include <string>
#include <unistd.h>
#include "message_helpers.h"
#include <fstream>
const char *path="/home/tinker/HOMER/vicon_data.txt";

using namespace std;

// simple data struct to hold a subject's information
struct subject{
  string name;
  int id;
  int quality;
  double r[4];
  double q[4];
};

// simple data struct to hold a single frame from the vicon broadcaster
struct vframe{
  int frame;
  subject subjects[20]; /// maximum of 20 subjects
  int nsubjects;
};

vframe stringparser(string msg,bool verbosef=false){
  /**
    * @brief: Function to parse a message from the broadcaster and return a populated vframe object
    *
    * Passes by reference, but we don't care about being efficient here
    */
  // create vicon frame
  vframe viconframe;
  // parse the frame number
  size_t frameend = msg.find(";");
  viconframe.frame = stoi(msg.substr(0,frameend));
/*  if (verbosef){
    cout << viconframe.frame << endl;
  }
*/  // add two to skip the colon and endline chars
  string line = msg.substr(frameend+2);
  int k = 0;
  for(k = 0;k<20;k++){
    //
    viconframe.subjects[k].name = line.substr(0,line.find(" "));
    size_t comma = line.find(",");
    viconframe.subjects[k].id = stoi(line.substr(line.find(" "),comma));
    // find the next comma
    size_t comma2 = line.find(",",comma+1);
    viconframe.subjects[k].quality = stoi(line.substr(comma+1,comma2-comma));
    // continue
    for (int i = 0;i<3;i++){
      comma = comma2;
      comma2 = line.find(",",comma+1);
      viconframe.subjects[k].r[i] = stod(line.substr(comma+1,comma2-comma));
    }
    for (int i = 0;i<4;i++){
      comma = comma2;
      comma2 = line.find(",",comma+1);
      viconframe.subjects[k].q[i] = stod(line.substr(comma+1,comma2-comma));
    }
/*    if (verbosef){
      cout << viconframe.subjects[k].name << "\t" << viconframe.subjects[k].id << "\t" << viconframe.subjects[k].quality;
      cout << "\t" << viconframe.subjects[k].r[0] << "\t" << viconframe.subjects[k].r[1] << "\t" << viconframe.subjects[k].r[2];
      cout << "\t" << viconframe.subjects[k].q[0] << "\t" << viconframe.subjects[k].q[1] << "\t" << viconframe.subjects[k].q[2] << "\t" << viconframe.subjects[k].q[3] << endl;
    }
*/    // read until ";" or until end
    size_t subjectend = line.find(";");
    // check if subjectend == len(line)
    if (subjectend == line.length()-2){
      // this is the last line
      break;
    }
    if (subjectend==string::npos){
      break;
    }
    line = line.substr(subjectend+2);
  }
  viconframe.nsubjects = k + 1;
/*  if (verbosef){
    cout << viconframe.nsubjects << " subjects" << endl;
  }
*/  return viconframe;
}

int main(){
  std::ofstream file;
  file.open(path,ios::out | ios::trunc);
  file.close();
  // zeromq context object wrapper
  context_helper context;
  // zeromq subscriber wrapper
  subscriber_helper sub(&context,"tcp://192.168.1.83:5563");
  // message buffer
  char msg[1024];
  // run until sigint
  while(6>5){
    // read while in the loop
    if (sub.recv()!=-1){
      std::ofstream file(path,ios::out | ios::app);
      if(file.is_open()){
      // copy message
      sub.get_msg(msg);
      // parse message
      string msg2 = msg;
      vframe viconframe = stringparser(msg2);
      cout << "Frame: " << viconframe.frame << endl;
      for(int k = 0;k<viconframe.nsubjects;k++){
        cout << viconframe.subjects[k].name << "," << viconframe.subjects[k].id << "," << viconframe.subjects[k].quality;
	file << "\n" << viconframe.subjects[k].name ;
        cout << "," << viconframe.subjects[k].r[0] << "," << viconframe.subjects[k].r[1] << "," << viconframe.subjects[k].r[2];
        file << "," << viconframe.subjects[k].r[0] << "," << viconframe.subjects[k].r[1] << "," << viconframe.subjects[k].r[2];
        cout << "," << viconframe.subjects[k].q[0] << "," << viconframe.subjects[k].q[1] << "," << viconframe.subjects[k].q[2] << "," << viconframe.subjects[k].q[3] << endl;
        file << "," << viconframe.subjects[k].q[0] << "," << viconframe.subjects[k].q[1] << "," << viconframe.subjects[k].q[2] << "," << viconframe.subjects[k].q[3];
      }
      }	
      file.close();
    }
    else{
      // otherwise, no message
      cout << "Cannot read from IP: 192.168.1.83 on port 5563\n";
      usleep(20e3);
    }
  }
  return 0;
}
