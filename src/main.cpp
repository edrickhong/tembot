#include "ttype.h"
#include "mode.h"

#include "pparse.h"
#include "ffileio.h"

#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"

#include "aallocator.h"

//TODO: abstract away sockets


void GetDNSHosts(s8** in_hosts,u32* in_count){
	auto file = FOpenFile("/etc/resolv.conf",F_FLAG_READONLY);

	_kill("",!file);

	ptrsize fsize = 0;
	auto buff = FReadFileToBuffer(file,&fsize);

	b32 found_host = false;

	for(ptrsize i = 0; i < fsize; i++){
		PSkipWhiteSpace(buff,&i);

		s8 c = buff[i];

		if(c == '#'){
			PSkipLine(buff,&i);
			i--;
			continue;
		}

		s8 inbuff[128] = {};
		u32 count = 0;

		PGetChunk(inbuff,buff,&i,&count);

		b32 is_nameserver = 
			PHashString(inbuff) == PHashString("nameserver");

		if(is_nameserver){
			found_host = true;
		}

		else if(found_host){
			//TODO: parse the name server
			if(in_hosts){
				auto s = in_hosts[*in_count];
				memcpy((void*)s,inbuff,count + 1);
			}

			(*in_count)++;
			found_host = false;
		}

		else{
			PSkipLine(buff,&i);
			i--;
			continue;
		}

	}

	unalloc(buff);

	FCloseFile(file);
}

void DNSResolveDomain(s8* hosts, s8* url){

	s32 sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	//TODO: implement our own version of this:https://pubs.opengroup.org/onlinepubs/009695399/functions/inet_addr.html
	//break here CURLcode resolve_server
}


s32 main(s32 argc,const s8** argv){

#define _total_hosts 8

	s8* hosts[_total_hosts] = {};
	u32 count = 0;

	s8 buffer[32 * _total_hosts] = {};

	for(u32 i = 0; i < _arraycount(hosts); i++){
		hosts[i] = &buffer[i * 32];
	}

	GetDNSHosts(hosts,&count);

	const s8* url = "https://discordapp.com/api/v6";
	const s8* gateway = "/gateway";

	//NOTE: We first need to connect to discordapp.com itself
	//port 443, then we need to send a payload to the gateway


	DNSResolveDomain(hosts[0],(s8*)url);

	s32 sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr = {0};

	if(soc < 0){
		return -1;
	}


	return 0;
}
