#include "IBackend.h"

namespace jam
{
	IBackend::IBackend()
	{
		this->currentScene = nullptr;
		this->render = nullptr;
	}
}
