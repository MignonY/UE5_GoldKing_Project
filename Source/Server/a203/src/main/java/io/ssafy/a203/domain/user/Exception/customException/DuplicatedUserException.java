package io.ssafy.a203.domain.user.Exception.customException;


import io.ssafy.a203.domain.user.Exception.ExceptionCode;

public class DuplicatedUserException extends RuntimeException {
    public DuplicatedUserException() {
        super(ExceptionCode.DUPLICATED_USER_EXCEPTION.getErrorMessage());
    }
    public DuplicatedUserException(String message) {
        super(message);
    }
}

