package io.ssafy.a203.domain.user.Exception;

import io.ssafy.a203.domain.user.Exception.customException.DuplicatedUserException;
import io.ssafy.a203.domain.user.Exception.customException.NoSuchStatException;
import io.ssafy.a203.domain.user.Exception.customException.NoSuchUserException;
import jakarta.servlet.http.HttpServletResponse;
import lombok.RequiredArgsConstructor;
import org.apache.tomcat.util.http.fileupload.impl.FileSizeLimitExceededException;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

@RequiredArgsConstructor
@RestControllerAdvice("site.doggyyummy.gaebap")
public class ExceptionController {


    @ExceptionHandler(NoSuchUserException.class)
    public ResponseEntity<String> handleNoSuchUserException (NoSuchUserException err){
        return getResponseEntity(ExceptionCode.NO_SUCH_USER_EXCEPTION);
    }

    @ExceptionHandler(NoSuchStatException.class)
    public ResponseEntity<String> handleNoSuchStatException (NoSuchStatException err){
        return getResponseEntity(ExceptionCode.NO_SUCH_STAT_EXCEPTION);
    }

    @ExceptionHandler(DuplicatedUserException.class)
    public ResponseEntity<String> handleDuplicatedUserException (DuplicatedUserException err){
        return getResponseEntity(ExceptionCode.DUPLICATED_USER_EXCEPTION);
    }
    public static ResponseEntity<String> getResponseEntity(ExceptionCode code){
        return ResponseEntity
                .status(code.getErrorCode())
                .body(code.getErrorMessage());
    }
}

