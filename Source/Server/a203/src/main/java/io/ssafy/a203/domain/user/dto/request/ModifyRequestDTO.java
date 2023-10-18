package io.ssafy.a203.domain.user.dto.request;


import io.ssafy.a203.domain.user.entity.Member;
import lombok.AllArgsConstructor;
import lombok.Getter;

@Getter
@AllArgsConstructor
public class ModifyRequestDTO {

    private String puid;
    private String signUpNickname;
    public static Member toEntity(ModifyRequestDTO modifyRequestDTO){
        return Member.builder()
                    .puid(modifyRequestDTO.getPuid())
                    .nickname(modifyRequestDTO.getSignUpNickname())
                .build();
    }

}
