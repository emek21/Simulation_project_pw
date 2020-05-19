#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <vector>
using std::vector;

class Channel
{
	// Represents channel object helps with collision detection
public:
	// Constructors and destructors
	Channel();
	~Channel() = default;
	// Checking channel status
	bool IsEmpty() const;
	// Blocking channel by trx
	void BlockChannel(int id);
	// Unblocking channell by trx
	void UnblockChannel(int id);
	// Detecting collision
	bool DetectCollision() const;
	// Return who has collision 
	vector<int> CollsionTrx() const;
	
private:
	// Vector for Transciever's process to inform other about collision
	vector<int> id_transcievers;
	
};

#endif//CHANNEL_H

