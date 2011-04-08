/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#include "CpAudio.hpp"
#include "CpSound.hpp"
#include "CpPlayer.hpp"

int main(void) {
	CpAudio* audio = CpAudio::Open();
	CpSound sound0, sound1;
	CpPlayer player0;
	CpPlayer player1;

	sound0.Load("/usr/share/sounds/purple/send.wav");
	sound1.Load("/usr/share/sounds/purple/alert.wav");

	player0.Play(&sound0);
	player0.Wait();
	
	player0.Play(&sound0);
	player1.Play(&sound1);
	player0.Wait();
	player1.Wait();

	audio->Close();

	return 0;
}
