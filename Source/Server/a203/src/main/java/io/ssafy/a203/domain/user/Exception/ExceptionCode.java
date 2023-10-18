package io.ssafy.a203.domain.user.Exception;

import lombok.Getter;

@Getter
public enum ExceptionCode {

    NO_SUCH_USER_EXCEPTION(404,"해당 유저는 존재하지 않습니다"),
    NO_SUCH_STAT_EXCEPTION(404,"해당 통계는 존재하지 않습니다"),
    DUPLICATED_USER_EXCEPTION(400,"이미 가입한 device ID입니다");

    private final Integer errorCode;
    private final String errorMessage;

    ExceptionCode(Integer errorCode, String errorMessage){
        this.errorCode = errorCode;
        this.errorMessage = errorMessage;
    }


}

