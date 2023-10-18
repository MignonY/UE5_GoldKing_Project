package io.ssafy.a203.domain.user.dto.response;

import io.ssafy.a203.domain.user.entity.Member;
import lombok.Builder;
import lombok.Getter;


@Builder
@Getter
public class MemberResponseDTO {
    private String nickname;

    public static MemberResponseDTO toDTO(Member member){
        return MemberResponseDTO.builder()
                .nickname(member.getNickname())
                .build();
    }
}
