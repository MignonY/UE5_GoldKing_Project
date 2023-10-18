package io.ssafy.a203.domain.user.service;

import io.ssafy.a203.domain.user.entity.Member;

import java.util.List;

public interface MemberService {

    //회원가입
    void signUp(Member member) throws Exception;

    boolean existsByPuid(String puid);

    Member findByPuid(String puid) throws Exception;

    Member modifyNickname(Member modifyMember) throws Exception;

    List<Member> rank(String stat) throws Exception;

    void update(Member updateMember) throws Exception;

}
