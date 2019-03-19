#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	for (uint k = 0; k < m_uEntityCount; k++)
	{
		MyEntity* tempEntity = m_entityList[k];
		SafeDelete(tempEntity);
	}

	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//loop through and check for ID
	for (uint k = 0; k < m_entityList.size(); k++)
	{
		//return index if found
		if (m_entityList[k]->GetUniqueID() == a_sUniqueID)
			return k;
	}

	//return -1 if not found
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	//if list is empty
	if (m_entityList.size() == 0)
		return nullptr;
	
	//if index out of bounds, grab the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	//temp entity to hold entity we're looking for
	MyEntity* tempEntity = MyEntity::GetEntity(a_sUniqueID);

	//if we found the entity
	if (tempEntity)
		return tempEntity->GetModel();

	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	//if list is empty
	if (m_entityList.size() == 0)
		return nullptr;

	//if index out of bounds, grab the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetRigidBody();

	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	//temp entity to hold entity we're looking for
	MyEntity* tempEntity = MyEntity::GetEntity(a_sUniqueID);

	//if we found the entity
	if (tempEntity)
		return tempEntity->GetRigidBody();

	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	//if list is empty
	if (m_entityList.size() == 0)
		return IDENTITY_M4;

	//if index out of bounds, grab the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetModelMatrix();

}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	//temp entity to hold entity we're looking for
	MyEntity* tempEntity = MyEntity::GetEntity(a_sUniqueID);

	//if we found the entity
	if (tempEntity)
		return tempEntity->GetModelMatrix();

	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	//temp entity to hold entity we're looking for
	MyEntity* tempEntity = MyEntity::GetEntity(a_sUniqueID);

	//if we found the entity
	if (tempEntity)
		tempEntity->SetModelMatrix(a_m4ToWorld);
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	//if list is empty
	if (m_entityList.size() == 0)
		return;

	//if index out of bounds, grab the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	for (uint k = 0; k < m_uEntityCount - 1; k++)
	{
		for (uint i = k + 1; i < m_uEntityCount; i++)
		{
			m_entityList[k]->IsColliding(m_entityList[i]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	//temp entity to store data
	MyEntity* tempEntity = new MyEntity(a_sFileName, a_sUniqueID);

	//if entity has been generated
	if (tempEntity->IsInitialized())
	{
		m_entityList.push_back(tempEntity);
		m_uEntityCount = m_entityList.size();
	}
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//if list is empty
	if (m_entityList.size() == 0)
		return;

	//if index out of bounds, grab the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//swap the entity we want to delete with the last one in the list for easier pop_back
	if (a_uIndex != m_uEntityCount - 1)
		std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);

	//grab entity
	MyEntity* tempEntity = m_entityList[m_uEntityCount - 1];

	//delete
	SafeDelete(tempEntity);

	//pop back empty index
	m_entityList.pop_back();

	//decrement count
	m_uEntityCount--;
	return;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	//grab entity's index by searching for uniqueID
	int entityIndex = GetEntityIndex(a_sUniqueID);

	//remove said entity
	RemoveEntity(entityIndex);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	//if list is empty
	if (m_entityList.size() == 0)
		return "";

	//if index out of bounds, grab the last one
	if (a_uIndex >= m_entityList.size())
		a_uIndex = m_entityList.size() - 1;

	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	//if list is empty
	if (m_entityList.size() == 0)
		return nullptr;

	//if index out of bounds, grab the last one
	if (a_uIndex >= m_entityList.size())
		a_uIndex = m_entityList.size() - 1;

	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//if index out of bounds, add the entire list
	if (a_uIndex >= m_uEntityCount)
	{
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; a_uIndex++)
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
	else
	{
		//only add the specific entity
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	//temp entity to hold data
	MyEntity* tempEntity = MyEntity::GetEntity(a_sUniqueID);

	//if entity exists
	if (tempEntity)
		tempEntity->AddToRenderList(a_bRigidBody);
}
