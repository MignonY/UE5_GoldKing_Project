package io.ssafy.a203.domain.user.dto.request;

import io.ssafy.a203.domain.user.entity.Member;
import lombok.AllArgsConstructor;
import lombok.Getter;
import org.hibernate.sql.Update;

@Getter
@AllArgsConstructor
public class UpdateStatsRequestDTO {

    private String puid;
    private int gold;
    private int kill;
    private int playCount;

    public static Member toEntity(UpdateStatsRequestDTO updateStatsRequestDTO){
        return Member.builder()
                .puid(updateStatsRequestDTO.getPuid())
                .gold(updateStatsRequestDTO.getGold())
                .killCount(updateStatsRequestDTO.getKill())
                .playCount(updateStatsRequestDTO.getPlayCount())
                .build();
    }
}
