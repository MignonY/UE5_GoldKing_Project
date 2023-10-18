package io.ssafy.a203.domain.user.service;

import io.ssafy.a203.domain.user.Exception.customException.DuplicatedUserException;
import io.ssafy.a203.domain.user.Exception.customException.NoSuchStatException;
import io.ssafy.a203.domain.user.Exception.customException.NoSuchUserException;
import io.ssafy.a203.domain.user.entity.Member;
import io.ssafy.a203.domain.user.respository.MemberRepository;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Slf4j
@RequiredArgsConstructor
@Service
@Transactional
public class MemberServiceImpl implements MemberService {

    private final MemberRepository memberRepository;

    @Override
    public void signUp(Member signupMember) throws NoSuchUserException {
        if (memberRepository.existsByPuid(signupMember.getPuid())) {
            Member existMember = memberRepository.findByPuid(signupMember.getPuid()).orElseThrow(() -> new NoSuchUserException());
            existMember.setNickname(signupMember.getNickname());
            return;
        }
        Member member = Member.builder()
                            .puid(signupMember.getPuid())
                            .nickname(signupMember.getNickname())
                            .gold(0)
                            .killCount(0)
                            .playCount(0)
                        .build();
        memberRepository.save(member);
    }

    @Override
    public Member findByPuid(String puid) throws NoSuchUserException{
        Member member = memberRepository.findByPuid(puid).orElseThrow(() -> new NoSuchUserException());
        return member;
    }

    @Override
    public Member modifyNickname(Member modifyMember) throws NoSuchUserException{
        Member member = findByPuid(modifyMember.getPuid());
        member.setNickname((modifyMember.getNickname()));
        return member;
    }

    @Override
    public List<Member> rank(String stat) throws NoSuchStatException{
        if (stat.equals("GOLD")) return rankByGold();
        if (stat.equals("KILL")) return rankByKill();
        if (stat.equals("PLAYCOUNT")) return rankByPlayCount();
        throw new NoSuchStatException();
    }

    @Override
    public void update(Member updateMember) throws NoSuchUserException {
        Member member = findByPuid(updateMember.getPuid());
        member.setGold(member.getGold() + updateMember.getGold());
        member.setKillCount(member.getKillCount() + updateMember.getKillCount());
        member.setKillCount(member.getPlayCount() + updateMember.getPlayCount());
    }

    @Override
    public boolean existsByPuid(String puid){
        return memberRepository.existsByPuid(puid);
    }

    private List<Member> rankByGold(){
        return memberRepository.findTop10ByOrderByGoldDesc();
    }

    private List<Member> rankByKill(){
        return memberRepository.findTop10ByOrderByKillCountDesc();
    }

    private List<Member> rankByPlayCount(){
        return memberRepository.findTop10ByOrderByPlayCountDesc();
    }
}
