package io.ssafy.a203.domain.user.dto.request;


import io.ssafy.a203.domain.user.entity.Member;
import lombok.AllArgsConstructor;
import lombok.Getter;

@Getter
@AllArgsConstructor
public class SignupRequestDTO {

    private String puid;
    private String signUpNickname;

    public static Member toEntity(SignupRequestDTO signupRequestDTO){
        return Member.builder()
                    .puid(signupRequestDTO.getPuid())
                    .nickname(signupRequestDTO.getSignUpNickname())
                .build();
    }

}
