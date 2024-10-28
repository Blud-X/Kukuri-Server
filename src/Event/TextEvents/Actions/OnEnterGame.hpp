#pragma once
#include <Event/EventType.hpp>
#include <Event/EventPool.hpp>
#include <Packet/TextFunction.hpp>
#include <Packet/VariantFunction.hpp>

ACTION_EVENT("enter_game", OnEnterGame) {
    if (!pAvatar) return;

    VarList::OnConsoleMessage(pAvatar->Get(),
        fmt::format("`oWelcome to `wKukuri Tavern`o, {}!``", pAvatar->GetRawName()));
    pServer->GetWorldPool()->SendToWorldSelect(pAvatar);
    Logger::Print(DEBUG, "Player {} entered game", pAvatar->GetRawName());
}