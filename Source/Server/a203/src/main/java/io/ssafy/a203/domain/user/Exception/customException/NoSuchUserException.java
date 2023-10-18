package io.ssafy.a203.domain.user.Exception.customException;


import io.ssafy.a203.domain.user.Exception.ExceptionCode;

public class NoSuchUserException extends RuntimeException {
    public NoSuchUserException() {
        super(ExceptionCode.NO_SUCH_USER_EXCEPTION.getErrorMessage());
    }

    public NoSuchUserException(String message) {
        super(message);
    }
}

