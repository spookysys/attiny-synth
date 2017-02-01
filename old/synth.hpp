#pragma once
#include "instruments.hpp"


class EchoTracks
{
	Bass bass;
	union {
		Lead lead;
		Fx fx;
	}
	


};


class OtherTracks
{
	BassDrum bd;
public:
};


class Tracks
{
	OtherTracks other_tracks;
	EchoTracks  echo_tracks;
	EchoTracks  echo_tracks_echo1;
	EchoTracks  echo_tracks_echo2;
public:
	void step
};


class Song
{
	Tracks tracks;
public:
	

};
