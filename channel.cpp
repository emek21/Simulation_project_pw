#include "channel.h"
#include <algorithm>

Channel::Channel()
{
	
}

// Function used by Transceiver
bool Channel::IsEmpty() const
{
	return id_transcievers.empty();
}

void Channel::BlockChannel(int id)
{
	id_transcievers.push_back(id);
}

void Channel::UnblockChannel(int id)
{
	id_transcievers.erase(std::remove(id_transcievers.begin(),
		id_transcievers.end(), id), id_transcievers.end());
}

bool Channel::DetectCollision() const
{
	if(id_transcievers.size() >= 2) return true;
	else return false;
}

vector<int> Channel::CollsionTrx() const
{
	return id_transcievers;
}
