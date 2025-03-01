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

	// 팝업 메시지가 있다면 Modal 팝업 표시
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

	ImVec2 windowPos = ImGui::GetWindowPos(); // 현재 창의 위치
	ImVec2 windowSize = ImGui::GetWindowSize();

	if (SERVER->Get_Connect())
	{
		static char idBuffer[128] = "";
		static char passBuffer[128] = "";

		// 창 가로 기준 중앙 정렬
		float centerX = (windowSize.x - 280) * 0.5f; // 280은 입력창 너비
		float posY = windowSize.y - 400; // 기존 Y 위치 유지

		ImGui::SetCursorPos(ImVec2(centerX, posY));
		ImGui::Text(u8"아이디:");
		ImGui::SetCursorPos(ImVec2(centerX, posY + 20)); // 위치 조정
		ImGui::InputText("##아이디", idBuffer, IM_ARRAYSIZE(idBuffer));

		ImGui::SetCursorPos(ImVec2(centerX, posY + 50));
		ImGui::Text(u8"비밀번호:");
		ImGui::SetCursorPos(ImVec2(centerX, posY + 70)); // 위치 조정
		ImGui::InputText("##비밀번호", passBuffer, IM_ARRAYSIZE(passBuffer));

		ImGui::SetCursorPos(ImVec2(centerX, posY + 120)); // 위치 조정

		if (ImGui::Button(u8"로그인", ImVec2(280, 40)))
		{
			string id = idBuffer;
			string password = passBuffer;
			SERVER->Send_Login(id, password);
			memset(idBuffer, 0, sizeof(idBuffer)); // 입력 버퍼 초기화
			memset(passBuffer, 0, sizeof(passBuffer)); // 입력 버퍼 초기화
		}

		ImGui::SetCursorPos(ImVec2(centerX, posY + 170)); // 위치 조정

		if (ImGui::Button(u8"회원가입", ImVec2(280, 40)))
		{
			string id = idBuffer;
			string password = passBuffer;
			SERVER->Send_SignUp(id, password);
			memset(idBuffer, 0, sizeof(idBuffer)); // 입력 버퍼 초기화
			memset(passBuffer, 0, sizeof(passBuffer)); // 입력 버퍼 초기화
		}
	}

	const char* msg = SERVER->Get_Connect() ? u8"연결되었습니다" : u8"연결 중...";
	ImVec2 textSize = ImGui::CalcTextSize(msg); // 텍스트 크기 계산

	float centerX = (windowSize.x - textSize.x) * 0.5f; // 창 중앙에 맞춤
	float posY = windowSize.y - 100; // 기존 y 위치 유지

	ImGui::SetCursorPos(ImVec2(centerX, posY));
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%s", msg);

	ImGui::End();
}

void Chat::Chatting()
{
	ImGui::SetWindowPos(u8"공지사항", ImVec2(0, 0));
	ImGui::SetWindowSize(u8"공지사항", ImVec2(300, 200));
	ImGui::SetWindowPos(u8"채팅", ImVec2(0, 200));
	ImGui::SetWindowSize(u8"채팅", ImVec2(300, 400));
	ImGui::SetWindowPos(u8"참가자", ImVec2(300, 0));
	ImGui::SetWindowSize(u8"참가자", ImVec2(100, 600));

	ImGui::Begin(u8"참가자", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs);

	vector<string> players = MESSAGE->GetPlayers();

	for (int i = 0; i < players.size(); i++)
	{
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), (players[i].c_str()));
	}

	ImGui::End();

	ImGui::Begin(u8"공지사항", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs);
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

	ImGui::Begin(u8"채팅", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

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

	ImVec2 windowPos = ImGui::GetWindowPos(); // 현재 창의 위치
	ImVec2 windowSize = ImGui::GetWindowSize();
	// 채팅 바를 하단에 배치
	ImGui::SetCursorPos(ImVec2(windowPos.x, windowPos.y + windowSize.y - 235));
	ImGui::Separator();

	// 채팅 입력 바
	static char inputBuf[256] = {}; // 입력 버퍼
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x * 2); // 간격을 제외한 전체 가로 크기

	ImGui::InputText("##input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue); // 작은 입력 필드

	if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter)) // 엔터 키를 누르면
	{
		ImGui::SetKeyboardFocusHere();
		if (inputBuf[0] != '\0') // 입력 내용이 비어있지 않다면
		{
			string Chat = inputBuf;
			SERVER->Send_Chat(Chat);
			memset(inputBuf, 0, sizeof(inputBuf)); // 입력 버퍼 초기화
		}
	}

	ImGui::End();
}

void Chat::PopUp(string msg)
{
	// Modal 팝업 열기
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