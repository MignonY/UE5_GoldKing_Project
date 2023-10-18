package io.ssafy.a203.domain.user.dto.response;


import io.ssafy.a203.domain.user.entity.Member;
import lombok.Builder;
import lombok.Getter;

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

@Getter
@Builder
public class LeaderboardResponseDTO {
    private int rank;
    private String nickname;
    private int score;

    public static List<LeaderboardResponseDTO> toDTO(List<Member> members, String stat){
        if (stat.equals("GOLD")){
            AtomicInteger index = new AtomicInteger();
            return members.stream().map(m->
                    LeaderboardResponseDTO.builder()
                            .rank(index.incrementAndGet())
                            .nickname(m.getNickname())
                            .score(m.getGold()).build()
            ).toList();
        }
        if (stat.equals("KILL")){
            AtomicInteger index = new AtomicInteger();
            return members.stream().map(m->
                    LeaderboardResponseDTO.builder()
                            .rank(index.incrementAndGet())
                            .nickname(m.getNickname())
                            .score(m.getKillCount()).build()
            ).toList();
        }
        if (stat.equals("PLAYCOUNT")){
            AtomicInteger index = new AtomicInteger();
            return members.stream().map(m->
                    LeaderboardResponseDTO.builder()
                            .rank(index.incrementAndGet())
                            .nickname(m.getNickname())
                            .score(m.getPlayCount()).build()
            ).toList();
        }
        return null;
    }
}
