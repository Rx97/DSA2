#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	//m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);

	//2d array to hold organization info to where to draw cubes to create invader
	invaderBlueprint = new int*[8];
	invaderBlueprint[0] = new int[11]{ 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 };
	invaderBlueprint[1] = new int[11]{ 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };
	invaderBlueprint[2] = new int[11]{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0 };
	invaderBlueprint[3] = new int[11]{ 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0 };
	invaderBlueprint[4] = new int[11]{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	invaderBlueprint[5] = new int[11]{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 };
	invaderBlueprint[6] = new int[11]{ 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1 };
	invaderBlueprint[7] = new int[11]{ 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0 };

	//:thinking: can i use just the matrix above and a single mesh and just render it multiple times
	//wherever it's needed instead during the update instead of having 46 different meshes?

	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 11; c++) {
			if (invaderBlueprint[r][c] == 1) {
				MyMesh* mesh = new MyMesh();
				mesh->GenerateCube(1.0f, C_BLACK);
				invaderMesh.push_back(mesh);
			}
		}
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(.7f, .7f, .7f));
	static float value = 0.0f;

	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * m4Translate;

	int currentIndex = 0;

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 11; c++)
		{
			if (invaderBlueprint[r][c] == 1) 
			{
				invaderMesh[currentIndex]->Render(m4Projection, m4View, glm::translate(m4Model, vector3(c, 8 - r, 0)));
				currentIndex++;
			}
		}
	}
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}