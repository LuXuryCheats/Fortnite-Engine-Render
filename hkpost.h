#pragma once

#include "stdafx.h"
#include "Misc.h"

namespace hkpost {
	inline void hkpostexecute();
}
inline auto RobotoFont = UObject::FindObject<UFont>(skCrypt("Font Roboto.Roboto"));
inline auto TestFont = UObject::FindObject<UFont>(skCrypt("Font BurbankSmall.BurbankSmall"));


inline BOOL(*LOS)(SDK::APlayerController* controller, SDK::AFortPlayerPawn* pawn, SDK::FVector* vp, bool alt) = nullptr;
inline BOOLEAN LineOFSightTo(SDK::APlayerController* controller, SDK::AFortPlayerPawn* pawn, SDK::FVector* vp) {
	return LOS(controller, pawn, vp, false);
}

inline uintptr_t(__fastcall* nigga_hook)(void*, FVector*, FRotator*) = 0;

inline PostRender_t RetPostRender;

inline bool World() {

	Basic::gWorld = (UWorld*)(*(uintptr_t*)Basic::UWorld_Offset);
	if (!Basic::gWorld) return false;
	if (!Basic::gWorld->OwningGameInstance) return false;
	if (!Basic::gWorld->OwningGameInstance->LocalPlayers[0]) return false;
	if (!Basic::gWorld->OwningGameInstance->LocalPlayers[0]->PlayerController) return false;

	Basic::LocalPlayer = Basic::gWorld->OwningGameInstance->LocalPlayers[0];
	Basic::LocalController = Basic::gWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;

	if (Basic::LocalController->AcknowledgedPawn) {
		world::bIsValid = true;
	}
	else {
		world::bIsValid = false;
	}

	return true;
}

inline void PostRender(UCanvas* pCanvas) {

	KeyNames();



	SDK::AFortPlayerPawn* Target;
	AFortPawn* closest_player = 0;
	float closest_distance = FLT_MAX;

	SDK::USkeletalMeshComponent* mesh = Target->Mesh;


	static SDK::UKismetSystemLibrary* KismetSystemLibrary = NULL;
	static SDK::UFortKismetLibrary* FortKismetLib = NULL;
	static SDK::UGameplayStatics* GameplayStatics = NULL;

	if (!KismetSystemLibrary) KismetSystemLibrary = (SDK::UKismetSystemLibrary*)SDK::UKismetSystemLibrary::StaticClass();
	if (!FortKismetLib) FortKismetLib = (SDK::UFortKismetLibrary*)SDK::UFortKismetLibrary::StaticClass();
	if (!GameplayStatics) GameplayStatics = (SDK::UGameplayStatics*)SDK::UGameplayStatics::StaticClass();


	if (!pCanvas) return;
	if (!World()) return;

	if (!Basic::MathLib) Basic::MathLib = (SDK::UKismetMathLibrary*)SDK::UKismetMathLibrary::StaticClass();

	if (Basic::LocalController->WasInputKeyJustPressed(KeyName::ToggleKey) & 1)
	{
		framework::show_menu = !framework::show_menu;
	}
	framework::render_menu(pCanvas);


	//PlayersLoop
	auto AllPlayers = FortKismetLib->STATIC_GetFortPlayerPawns(Basic::gWorld = (UWorld*)(*(uintptr_t*)Basic::UWorld_Offset));
	for (int i = 0; i < AllPlayers.Num(); i++)
	{


		float max = 999.f;
		SDK::AFortPlayerPawn* Actor = (AFortPlayerPawn*)AllPlayers[i];
		if (Actor == (AFortPlayerPawnAthena*)Basic::LocalController->K2_GetPawn())  continue;
		if (FortKismetLib->STATIC_OnSameTeam(Basic::LocalController->AcknowledgedPawn, Actor)) continue;
		if (Actor->IsDead()) continue;
		if (Actor->IsDBNO())continue;

		/*{
			//auto playerstate = Actor->PlayerState;
			auto Statics = Basic::gGameplayStatics;
			//SDK::FString Name;
			//Name = playerstate->GetPlayerName();
			pCanvas->K2_DrawText(RobotoFont, FString(Name.c_str()), FVector2D(namePosX - 12, namePosY - 8), FVector2D(1, 1), FLinearColor(0.f, 0.f, 0.f, 1.0f), 1.0f,
				FLinearColor(0.f, 0.f, 0.f, 1.0f), FVector2D(), true, true, false, FLinearColor(0.f, 0.f, 0.f, 1.0f));
			pCanvas->K2_DrawText(RobotoFont, FString(Name.c_str()), FVector2D(namePosX - 13, namePosY - 9), FVector2D(1, 1), FLinearColor(255.f, 0.f, 0.f, 1.0f), 1.0f,
				FLinearColor(255.f, 255.f, 255.f, 1.0f), FVector2D(), true, true, false, FLinearColor(255.f, 0.f, 0.f, 1.0f));
		}
		*/

		SDK::FVector viewPoint = { 0, 0, 0 };
		bool visible = LineOFSightTo(Basic::LocalController, Actor, &viewPoint) && (options::Aim::visible_check);

		auto OutlineColor = SDK::FLinearColor(0.f, 0.f, 0.f, 1.0f);
		auto CrosshairColor = SDK::FLinearColor(255.f, 255.f, 255.f, 1.0f);
		auto White = SDK::FLinearColor(255.f, 255.f, 255.f, 255.f);

		SDK::USkeletalMeshComponent* mesh = Actor->Mesh;
		RECT rDraw;
		bool bDrawBones = true;

		FVector2D vPos[100];
		std::vector<int> nBoneIDS = { 66 ,65 ,36 ,2 ,9 ,62,10,38,11,39,67,74,73,80,68,75,71,78,72,79 }; // when drawing bones
		std::vector<int> nBoneIDSSimple = { 66, 9, 62, 11, 39, 72, 79 };                                // when not drawing bones.
		std::vector<std::vector<int>> nBonePairs =
		{
		{99, 1},
		{1, 3},
		{2, 4},
		{-+2, 5},
		{4, 6},
		{5, 7},
		{6, 8},
		{7, 9},
		{3, 10},
		{3, 11},
		{10, 12},
		{11, 13},
		{12, 14},
		{13, 15},
		{14, 16},
		{15, 17},
		{16, 18},
		{17, 19}
		};

		size_t nIndex = 0;

		float minX = FLT_MAX;
		float maxX = -FLT_MAX;
		float minY = FLT_MAX;
		float maxY = -FLT_MAX;

		if (bDrawBones)
		{
			for (auto ID : nBoneIDS)
			{
				bool bResult = false;
				if (ID == 66)
				{
					FVector vWorldPos;
					if (hkfunctions::GetBoneWorld((ACharacter*)Actor, 66, &vWorldPos))
					{
						Basic::LocalController->ProjectWorldLocationToScreen(vWorldPos, false, &vPos[99]);

						vWorldPos.Z += 15.5f;
						bResult = Basic::LocalController->ProjectWorldLocationToScreen(vWorldPos, false, &vPos[nIndex]);
					}
				}
				else
				{
					nIndex++;
					bResult = hkfunctions::GetBoneScreen((ACharacter*)Actor, ID, &vPos[nIndex]);
				}

				if (bResult)
				{
					minX = min(vPos[nIndex].X, minX);
					maxX = max(vPos[nIndex].X, maxX);
					minY = min(vPos[nIndex].Y, minY);
					maxY = max(vPos[nIndex].Y, maxY);
				}
			}
		}
		else
		{
			for (auto ID : nBoneIDSSimple)
			{
				bool bResult = false;
				if (ID == 66)
				{
					FVector vWorldPos;
					if (hkfunctions::GetBoneWorld((ACharacter*)Actor, 66, &vWorldPos))
					{
						bResult = Basic::LocalController->ProjectWorldLocationToScreen(vWorldPos, false, &vPos[99]);

						vWorldPos.Z += 15.5f;
						bResult = Basic::LocalController->ProjectWorldLocationToScreen(vWorldPos, false, &vPos[nIndex]);
					}
				}
				else
				{
					nIndex++;
					bResult = hkfunctions::GetBoneScreen((ACharacter*)Actor, ID, &vPos[nIndex]);
				}

				if (bResult)
				{
					minX = min(vPos[nIndex].X, minX);
					maxX = max(vPos[nIndex].X, maxX);
					minY = min(vPos[nIndex].Y, minY);
					maxY = max(vPos[nIndex].Y, maxY);
				}
			}
		}

		RECT rResult;
		rResult.top = minY;
		rResult.bottom = maxY - minY;
		rResult.left = minX;
		rResult.right = maxX - minX;

		rDraw = rResult;

		auto topLeft = FVector2D(minX - 3.0f, minY - 3.0f);
		auto bottomRight = FVector2D(maxX + 3.0f, maxY + 3.0f);

		auto centerTop = FVector2D((topLeft.X + bottomRight.X) / 2.0f + +10 + 1.65, topLeft.Y);

		auto theAdditionY = (bottomRight.Y - topLeft.Y) / 5;
		auto theAdditionX = (bottomRight.X - topLeft.X) / 3;

		auto topRight = FVector2D(bottomRight.X, topLeft.Y);

		auto bottomLeft = FVector2D(topLeft.X, bottomRight.Y);

		float THICKERTHANASNICKER = 2.f;
		float DummyThic = 1.f;
		SDK::AFortPlayerPawn* mypawn = (SDK::AFortPlayerPawn*)Basic::gWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->Pawn;
		auto mapawn = (SDK::AFortPlayerPawnAthena*)Basic::LocalController->K2_GetPawn();

		auto BoxColor = visible ? SDK::FLinearColor(0.f, 255.f, 0.f, 1.0f) : SDK::FLinearColor(255.f, 0.f, 0.f, 1.0f);

		if (options::Aim::drawfov) {
			hkrender::DrawCircle(pCanvas, width / 2, height / 2, options::Aim::fov_range, 60, FLinearColor(255.0f, 255.0f, 255.0f, 255.0f));
		}

		if (options::Visuals::CornerBox) {
			if (options::Visuals::OutlineBox)
			{
				pCanvas->K2_DrawLine(FVector2D(topLeft.X, topLeft.Y), FVector2D(topLeft.X, topLeft.Y + theAdditionY), options::Visuals::BoxThickness + 2, OutlineColor); // Top Left Y
				pCanvas->K2_DrawLine(FVector2D(topLeft.X, topLeft.Y), FVector2D(topLeft.X + theAdditionX, topLeft.Y), options::Visuals::BoxThickness + 2, OutlineColor); // Top Left X

				pCanvas->K2_DrawLine(FVector2D(topRight.X, topRight.Y), FVector2D(topRight.X, topRight.Y + theAdditionY), options::Visuals::BoxThickness + 2, OutlineColor); // Top Right Y
				pCanvas->K2_DrawLine(FVector2D(topRight.X, topRight.Y), FVector2D(topRight.X - theAdditionX, topRight.Y), options::Visuals::BoxThickness + 2, OutlineColor); // Top Right X

				pCanvas->K2_DrawLine(FVector2D(bottomLeft.X, bottomLeft.Y), FVector2D(bottomLeft.X, bottomLeft.Y - theAdditionY), options::Visuals::BoxThickness + 2, OutlineColor); // Bottom Left Y
				pCanvas->K2_DrawLine(FVector2D(bottomLeft.X, bottomLeft.Y), FVector2D(bottomLeft.X + theAdditionX, bottomLeft.Y), options::Visuals::BoxThickness + 2, OutlineColor); // Bottom Left X

				pCanvas->K2_DrawLine(FVector2D(bottomRight.X, bottomRight.Y), FVector2D(bottomRight.X, bottomRight.Y - theAdditionY), options::Visuals::BoxThickness + 2, OutlineColor); // Bottom Right Y
				pCanvas->K2_DrawLine(FVector2D(bottomRight.X, bottomRight.Y), FVector2D(bottomRight.X - theAdditionX, bottomRight.Y), options::Visuals::BoxThickness + 2, OutlineColor); // Bottom Right X
			}
			pCanvas->K2_DrawLine(FVector2D(topLeft.X, topLeft.Y), FVector2D(topLeft.X, topLeft.Y + theAdditionY), options::Visuals::BoxThickness, BoxColor); // Top Left Y
			pCanvas->K2_DrawLine(FVector2D(topLeft.X, topLeft.Y), FVector2D(topLeft.X + theAdditionX, topLeft.Y), options::Visuals::BoxThickness, BoxColor); // Top Left X

			pCanvas->K2_DrawLine(FVector2D(topRight.X, topRight.Y), FVector2D(topRight.X, topRight.Y + theAdditionY), options::Visuals::BoxThickness, BoxColor); // Top Right Y
			pCanvas->K2_DrawLine(FVector2D(topRight.X, topRight.Y), FVector2D(topRight.X - theAdditionX, topRight.Y), options::Visuals::BoxThickness, BoxColor); // Top Right X

			pCanvas->K2_DrawLine(FVector2D(bottomLeft.X, bottomLeft.Y), FVector2D(bottomLeft.X, bottomLeft.Y - theAdditionY), options::Visuals::BoxThickness, BoxColor); // Bottom Left Y
			pCanvas->K2_DrawLine(FVector2D(bottomLeft.X, bottomLeft.Y), FVector2D(bottomLeft.X + theAdditionX, bottomLeft.Y), options::Visuals::BoxThickness, BoxColor); // Bottom Left X

			pCanvas->K2_DrawLine(FVector2D(bottomRight.X, bottomRight.Y), FVector2D(bottomRight.X, bottomRight.Y - theAdditionY), options::Visuals::BoxThickness, BoxColor); // Bottom Right Y
			pCanvas->K2_DrawLine(FVector2D(bottomRight.X, bottomRight.Y), FVector2D(bottomRight.X - theAdditionX, bottomRight.Y), options::Visuals::BoxThickness, BoxColor); // Bottom Right X
		}


		if (options::Visuals::BoxESP) {

			if (options::Visuals::OutlineBox)
			{
				hkfunctions::drawRect(pCanvas, FVector2D(topLeft), FVector2D(bottomRight), OutlineColor, options::Visuals::BoxThickness + 2);
			}
			hkfunctions::drawRect(pCanvas, FVector2D(topLeft), FVector2D(bottomRight), BoxColor, options::Visuals::BoxThickness);
		}


		if (mesh)
		{
			auto playerstate = Actor->PlayerState;
			auto Statics = Basic::gGameplayStatics;


			if (playerstate)
			{

				FVector Mainhead = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_HEAD), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector head = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_NECK), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector chest = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_CHEST), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lshoulder = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_SHOULDER_2), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rshoulder = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_SHOULDER), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lelbow = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_ELBOW), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector relbow = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_ELBOW), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lhand = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_HAND_ROOT_1), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rhand = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_HAND_ROOT_1), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector pelvis = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_PELVIS_1), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lleg = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_LEG_ROOT), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rleg = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_LEG_ROOT), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lknee = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_KNEE), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rknee = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_KNEE), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lfoot = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_FOOT_MID), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rfoot = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_FOOT_MID), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lfootroot = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_FOOT_ROOT), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rfootroot = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_FOOT_ROOT), Basic::LocalController->PlayerCameraManager->GetCameraRotation());

				FVector lfinger1 = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_FINGER_1), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lfinger2 = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_FINGER_2), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lfinger3 = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_FINGER_3), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector lfinger4 = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_L_FINGER_4_), Basic::LocalController->PlayerCameraManager->GetCameraRotation());

				FVector rfinger1 = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_FINGER_1), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rfinger2 = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_FINGER_2), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rfinger3 = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_FINGER_3), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
				FVector rfinger4 = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, eBone::BONE_R_FINGER_4_), Basic::LocalController->PlayerCameraManager->GetCameraRotation());






				auto centerTop = FVector2D((topLeft.X + bottomRight.X) / 2.0f + +10 + 1.65, topLeft.Y);
				auto namePosX = centerTop.X;
				auto namePosY = centerTop.Y;
				auto centerBottom = FVector2D((bottomLeft.X + topRight.X) / 2.0f + +10 + 1.65, bottomLeft.Y);
				auto DistanceX = centerBottom.X;
				auto DistanceY = centerBottom.Y;
				if (options::Visuals::headdot)
				{
					hkfunctions::drawRect(pCanvas, FVector2D(Mainhead.X - 1, Mainhead.Y - 1), FVector2D(Mainhead.X + 1, Mainhead.Y + 1), White, 7);

				}
				if (options::Visuals::skeleton)
				{
					pCanvas->K2_DrawLine(FVector2D(Mainhead.X, Mainhead.Y), FVector2D(head.X, head.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(head.X, head.Y), FVector2D(chest.X, chest.Y), 1, White);

					pCanvas->K2_DrawLine(FVector2D(head.X, head.Y), FVector2D(rshoulder.X, rshoulder.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(head.X, head.Y), FVector2D(lshoulder.X, lshoulder.Y), 1, White);

					pCanvas->K2_DrawLine(FVector2D(lshoulder.X, lshoulder.Y), FVector2D(lelbow.X, lelbow.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(lelbow.X, lelbow.Y), FVector2D(lhand.X, lhand.Y), 1, White);

					pCanvas->K2_DrawLine(FVector2D(rshoulder.X, rshoulder.Y), FVector2D(relbow.X, relbow.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(relbow.X, relbow.Y), FVector2D(rhand.X, rhand.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(head.X, head.Y), FVector2D(chest.X, chest.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(chest.X, chest.Y), FVector2D(pelvis.X, pelvis.Y), 1, White);

					pCanvas->K2_DrawLine(FVector2D(pelvis.X, pelvis.Y), FVector2D(lleg.X, lleg.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(lleg.X, lleg.Y), FVector2D(lknee.X, lknee.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(lknee.X, lknee.Y), FVector2D(lfoot.X, lfoot.Y), 1, White);

					pCanvas->K2_DrawLine(FVector2D(pelvis.X, pelvis.Y), FVector2D(rleg.X, rleg.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(rleg.X, rleg.Y), FVector2D(rknee.X, rknee.Y), 1, White);
					pCanvas->K2_DrawLine(FVector2D(rknee.X, rknee.Y), FVector2D(rfoot.X, rfoot.Y), 1, White);

					//	pCanvas->K2_DrawLine(FVector2D(lhand.X, lhand.Y), FVector2D(lfinger1.X, lfinger1.Y), 1, White);
					//	pCanvas->K2_DrawLine(FVector2D(lhand.X, lhand.Y), FVector2D(lfinger2.X, lfinger2.Y), 1, White);
					//	pCanvas->K2_DrawLine(FVector2D(lhand.X, lhand.Y), FVector2D(lfinger3.X, lfinger3.Y), 1, White);
					//	pCanvas->K2_DrawLine(FVector2D(lhand.X, lhand.Y), FVector2D(lfinger4.X, lfinger4.Y), 1, White);

					//	pCanvas->K2_DrawLine(FVector2D(rhand.X, rhand.Y), FVector2D(rfinger1.X, rfinger1.Y), 1, White);
					//	pCanvas->K2_DrawLine(FVector2D(rhand.X, rhand.Y), FVector2D(rfinger2.X, rfinger2.Y), 1, White);
					//	pCanvas->K2_DrawLine(FVector2D(rhand.X, rhand.Y), FVector2D(rfinger3.X, rfinger3.Y), 1, White);
					//	pCanvas->K2_DrawLine(FVector2D(rhand.X, rhand.Y), FVector2D(rfinger4.X, rfinger4.Y), 1, White);

				}
				if (options::Misc::FovChanger)
				{
					Basic::LocalController->FOV(options::Misc::FOV);
				}

				if (options::Misc::CustomCrosshair)
				{

					pCanvas->K2_DrawLine(FVector2D(width / 2 + 5, height / 2), FVector2D(width / 2 + options::Misc::CrosshairSize, height / 2), 3, CrosshairColor);
					pCanvas->K2_DrawLine(FVector2D(width / 2 - 5, height / 2), FVector2D(width / 2 - options::Misc::CrosshairSize, height / 2), 3, CrosshairColor);
					pCanvas->K2_DrawLine(FVector2D(width / 2, height / 2 + 5), FVector2D(width / 2, height / 2 + options::Misc::CrosshairSize), 3, CrosshairColor);
					pCanvas->K2_DrawLine(FVector2D(width / 2, height / 2 - 5), FVector2D(width / 2, height / 2 - options::Misc::CrosshairSize), 3, CrosshairColor);
				}
				if (options::Visuals::snaplines)
				{
					pCanvas->K2_DrawLine(FVector2D(960, 0), FVector2D(namePosX - 12, namePosY - 5), 0.7, FLinearColor(255.f, 255.f, 255.f, 1.0f)); // Top Left Y
				}



				if (options::Visuals::Distance)
				{

					auto Camera = Basic::LocalController->PlayerCameraManager->GetCameraLocation();
					FVector root_pos = Actor->K2_GetActorLocation();
					float dist_to_pawn = calc_distance(Camera, root_pos);

					std::wstring distance = std::to_wstring((int)dist_to_pawn); distance += std::wstring(skCrypt(L"M"));


					if (options::Visuals::OutlineText)
					{
						pCanvas->K2_DrawText(RobotoFont, FString(distance.c_str()), FVector2D(namePosX - 12, namePosY - 21), FVector2D(1, 1), FLinearColor(0.f, 0.f, 0.f, 1.0f), 1.0f,
							FLinearColor(0.f, 0.f, 0.f, 1.0f), FVector2D(), true, true, false, FLinearColor(0.f, 0.f, 0.f, 1.0f));
					}
					pCanvas->K2_DrawText(RobotoFont, FString(distance.c_str()), FVector2D(namePosX - 13, namePosY - 22), FVector2D(1, 1), FLinearColor(255.f, 255.f, 255.f, 1.0f), 1.0f,
						FLinearColor(255.f, 255.f, 255.f, 1.0f), FVector2D(), true, true, false, FLinearColor(255.f, 255.f, 255.f, 1.0f));
				}

				if (options::Visuals::NameESP)
				{

					SDK::FString Name;
					Name = playerstate->GetPlayerName();
					if (options::Visuals::OutlineText)
					{
						pCanvas->K2_DrawText(RobotoFont, FString(Name.c_str()), FVector2D(namePosX - 12, namePosY - 8), FVector2D(1, 1), FLinearColor(0.f, 0.f, 0.f, 1.0f), 1.0f,
							FLinearColor(0.f, 0.f, 0.f, 1.0f), FVector2D(), true, true, false, FLinearColor(0.f, 0.f, 0.f, 1.0f));
					}
					pCanvas->K2_DrawText(RobotoFont, FString(Name.c_str()), FVector2D(namePosX - 13, namePosY - 9), FVector2D(1, 1), FLinearColor(255.f, 255.f, 255.f, 1.0f), 1.0f,
						FLinearColor(255.f, 255.f, 255.f, 1.0f), FVector2D(), true, true, false, FLinearColor(255.f, 255.f, 255.f, 1.0f));

				}



				if (visible || !options::Aim::visible_check)
				{
					SDK::FVector2D wnd_size = SDK::FVector2D(width, height);
					FVector target = hkfunctions::W2S(hkfunctions::GetBoneLocByIdx(mesh, (int)options::Aim::head && (int)options::Aim::neck && (int)options::Aim::chest && (int)options::Aim::pelvis), Basic::LocalController->PlayerCameraManager->GetCameraRotation());
					auto dist = GetDistance2D(wnd_size / 2, target);
					//	LookAt(Basic::LocalController, target);

					if (dist < options::Aim::fov_range && dist < closest_distance)
					{

						closest_distance = dist;
						closest_player = Actor;
					}

				}
			}
		}
	}


	SDK::AFortWeapon* FortWeapon = (SDK::AFortWeapon*)Basic::FortWeapon;
	SDK::AFortPlayerPawn* MyPawn = (SDK::AFortPlayerPawn*)Basic::LocalController->AcknowledgedPawn;
	SDK::AFortPlayerPawn* mypawn = (SDK::AFortPlayerPawn*)Basic::gWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->Pawn;
	auto mapawn = (SDK::AFortPlayerPawnAthena*)Basic::LocalController->K2_GetPawn();
	auto Camera = Basic::LocalController->PlayerCameraManager;
	if (options::Misc::RapidFire)
	{
		if (mapawn && mypawn && Basic::LocalController->AcknowledgedPawn->CurrentWeapon && Basic::LocalController->AcknowledgedPawn)
		{
			if (Basic::LocalController->AcknowledgedPawn->IsA(SDK::AFortPawn::StaticClass()))
			{
				if (Basic::LocalController->AcknowledgedPawn->CurrentWeapon->GetWeaponDataItemType() == SDK::EFortItemType::WeaponRanged)
				{
					Basic::LocalController->AcknowledgedPawn->CurrentWeapon->LastFireTime = Basic::LocalController->AcknowledgedPawn->CurrentWeapon->LastFireTime - 0.003f;
				}
			}

		}
	}

	if (options::Misc::bNoBloom) {

		auto MyPawn = Basic::gWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->Pawn;
		auto EditPawn = (SDK::AFortPlayerPawnAthena*)Basic::LocalController->K2_GetPawn();
		if (EditPawn && MyPawn) {

			if (Basic::LocalController->AcknowledgedPawn->CurrentWeapon && Basic::LocalController->AcknowledgedPawn && Basic::LocalController->IsInputKeyDown(KeyName::Left)) // while the player is valid and while the key is down it executes the code below
			{
				*(float*)((PBYTE)EditPawn->CurrentWeapon + 0x30) = 0.11f;

			}
		}
	}

	if (options::Aim::memory)
	{

		if (bAimingLock)
			bAimingLock = false;

		if (closest_player && Basic::LocalController->IsInputKeyDown(KeyName::Right) && Basic::LocalController->AcknowledgedPawn->CurrentWeapon)
		{
			if (Basic::LocalController->AcknowledgedPawn->CurrentWeapon->GetWeaponDataItemType() == SDK::EFortItemType::WeaponRanged)
			{

				hkfunctions::SetAim(Camera, (SDK::AFortPlayerPawn*)closest_player);

			}
		}
	}
}




inline void hkPostRender(UObject* pObject, UCanvas* pCanvas, SDK::APlayerCameraManager* CamManager, float MaxDist = 250) {

	if (pCanvas) {

		width = pCanvas->SizeX;
		height = pCanvas->SizeY;

		PostRender(pCanvas);
	}

	return RetPostRender(pObject, pCanvas, CamManager, MaxDist);

}

inline void hkpost::hkpostexecute() {

	PostRender_t* pr_ptr;

	pr_ptr = (PostRender_t*)((DWORD64)GetModuleHandleW(NULL) + 0x6BA7C60); // PostRender Vtable.
	RetPostRender = (PostRender_t)*pr_ptr;
	*pr_ptr = hkPostRender;

	

	
}
