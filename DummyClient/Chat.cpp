#include "pch.h"
#include "Chat.h"
#include "Protocol.pb.h"

void Chat::Init()
{

}

void Chat::Update()
{
	switch (SERVER->Get_State())
	{
	case STATE::LOGIN:
		Login();
		break;
	case STATE::CHAT:
		Chatting();
		break;
	}

	// �˾� �޽����� �ִٸ� Modal �˾� ǥ��
	auto msg = MESSAGE->GetPopupMessage();
	if (msg != string())
		PopUp(msg);
}

void Chat::Render()
{
}

void Chat::Login()
{
	ImGui::SetWindowPos("Login", ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetWindowSize("Login", ImVec2(400, 600), ImGuiCond_Once);

	ImGui::Begin("Login", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImVec2 windowPos = ImGui::GetWindowPos(); // ���� â�� ��ġ
	ImVec2 windowSize = ImGui::GetWindowSize();

	if (SERVER->Get_Connect())
	{
		static char idBuffer[128] = "";
		static char passBuffer[128] = "";

		// â ���� ���� �߾� ����
		float centerX = (windowSize.x - 280) * 0.5f; // 280�� �Է�â �ʺ�
		float posY = windowSize.y - 400; // ���� Y ��ġ ����

		ImGui::SetCursorPos(ImVec2(centerX, posY));
		ImGui::Text(u8"���̵�:");
		ImGui::SetCursorPos(ImVec2(centerX, posY + 20)); // ��ġ ����
		ImGui::InputText("##���̵�", idBuffer, IM_ARRAYSIZE(idBuffer));

		ImGui::SetCursorPos(ImVec2(centerX, posY + 50));
		ImGui::Text(u8"��й�ȣ:");
		ImGui::SetCursorPos(ImVec2(centerX, posY + 70)); // ��ġ ����
		ImGui::InputText("##��й�ȣ", passBuffer, IM_ARRAYSIZE(passBuffer));

		ImGui::SetCursorPos(ImVec2(centerX, posY + 120)); // ��ġ ����

		if (ImGui::Button(u8"�α���", ImVec2(280, 40)))
		{
			string id = idBuffer;
			string password = passBuffer;
			SERVER->Send_Login(id, password);
			memset(idBuffer, 0, sizeof(idBuffer)); // �Է� ���� �ʱ�ȭ
			memset(passBuffer, 0, sizeof(passBuffer)); // �Է� ���� �ʱ�ȭ
		}

		ImGui::SetCursorPos(ImVec2(centerX, posY + 170)); // ��ġ ����

		if (ImGui::Button(u8"ȸ������", ImVec2(280, 40)))
		{
			string id = idBuffer;
			string password = passBuffer;
			SERVER->Send_SignUp(id, password);
			memset(idBuffer, 0, sizeof(idBuffer)); // �Է� ���� �ʱ�ȭ
			memset(passBuffer, 0, sizeof(passBuffer)); // �Է� ���� �ʱ�ȭ
		}
	}

	const char* msg = SERVER->Get_Connect() ? u8"����Ǿ����ϴ�" : u8"���� ��...";
	ImVec2 textSize = ImGui::CalcTextSize(msg); // �ؽ�Ʈ ũ�� ���

	float centerX = (windowSize.x - textSize.x) * 0.5f; // â �߾ӿ� ����
	float posY = windowSize.y - 100; // ���� y ��ġ ����

	ImGui::SetCursorPos(ImVec2(centerX, posY));
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%s", msg);

	ImGui::End();
}

void Chat::Chatting()
{
	ImGui::SetWindowPos(u8"��������", ImVec2(0, 0));
	ImGui::SetWindowSize(u8"��������", ImVec2(300, 200));
	ImGui::SetWindowPos(u8"ä��", ImVec2(0, 200));
	ImGui::SetWindowSize(u8"ä��", ImVec2(300, 400));
	ImGui::SetWindowPos(u8"������", ImVec2(300, 0));
	ImGui::SetWindowSize(u8"������", ImVec2(100, 600));

	ImGui::Begin(u8"������", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs);

	vector<string> players = MESSAGE->GetPlayers();

	for (int i = 0; i < players.size(); i++)
	{
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), (players[i].c_str()));
	}

	ImGui::End();

	ImGui::Begin(u8"��������", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs);
	const auto messages = MESSAGE->GetRecentMessages();

	if (!messages.empty())
	{
		for (int i = 0; i < messages.size(); i++)
		{
			if (messages[i].type == Protocol::PlayerType::PLAYERTYPE_MANAGER)
				ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), (messages[i].msg.c_str()));
		}
	}
	ImGui::End();

	ImGui::Begin(u8"ä��", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (!messages.empty())
	{
		for (int i = 0; i < messages.size(); i++)
		{
			if (messages[i].type == Protocol::PlayerType::PLAYERTYPE_USER)
			{
				string msg = messages[i].name + " : " + messages[i].msg;
				ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), msg.c_str());
			}

		}
	}

	ImVec2 windowPos = ImGui::GetWindowPos(); // ���� â�� ��ġ
	ImVec2 windowSize = ImGui::GetWindowSize();
	// ä�� �ٸ� �ϴܿ� ��ġ
	ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y + windowSize.y - 235));
	ImGui::Separator();

	// ä�� �Է� ��
	static char inputBuf[256] = {}; // �Է� ����
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x * 2); // ������ ������ ��ü ���� ũ��

	ImGui::InputText("##input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue); // ���� �Է� �ʵ�

	if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter)) // ���� Ű�� ������
	{
		ImGui::SetKeyboardFocusHere();
		if (inputBuf[0] != '\0') // �Է� ������ ������� �ʴٸ�
		{
			string Chat = inputBuf;
			SERVER->Send_Chat(Chat);
			memset(inputBuf, 0, sizeof(inputBuf)); // �Է� ���� �ʱ�ȭ
		}
	}

	ImGui::End();
}

void Chat::PopUp(string msg)
{
	// Modal �˾� ����
	if (!ImGui::IsPopupOpen("Warning"))
	{
		ImGui::OpenPopup("Warning");
	}

	if (ImGui::BeginPopupModal("Warning", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text(msg.c_str());
		if (ImGui::Button("Close", ImVec2(180, 0)))
		{
			ImGui::CloseCurrentPopup();
			MESSAGE->ClearPopup();
		}
		ImGui::EndPopup();
	}
}