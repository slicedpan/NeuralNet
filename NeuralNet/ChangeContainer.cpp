#include "StdAfx.h"
#include "ChangeContainer.h"


ChangeContainer::ChangeContainer(void)
{
}


ChangeContainer::~ChangeContainer(void)
{
	for (int i = 0; i < nodeChanges.size(); ++i)
	{
		delete nodeChanges[i];
	}
	for (int i = 0; i < netChanges.size(); ++i)
	{
		delete netChanges[i];
	}
}
