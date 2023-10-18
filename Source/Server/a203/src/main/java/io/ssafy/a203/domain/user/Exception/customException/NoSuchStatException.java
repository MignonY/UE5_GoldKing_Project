package io.ssafy.a203.domain.user.Exception.customException;


import io.ssafy.a203.domain.user.Exception.ExceptionCode;

public class NoSuchStatException extends RuntimeException {
    public NoSuchStatException() {
        super(ExceptionCode.NO_SUCH_STAT_EXCEPTION.getErrorMessage());
    }
    public NoSuchStatException(String message) {
        super(message);
    }
}

