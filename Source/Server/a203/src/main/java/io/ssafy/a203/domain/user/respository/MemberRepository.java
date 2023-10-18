package io.ssafy.a203.domain.user.respository;

import io.ssafy.a203.domain.user.entity.Member;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;
import java.util.Optional;

public interface MemberRepository extends JpaRepository<Member, Long> {

    Optional<Member> findByPuid(String puid);
    boolean existsByPuid(String puid);

    List<Member> findTop10ByOrderByGoldDesc();
    List<Member> findTop10ByOrderByKillCountDesc();
    List<Member> findTop10ByOrderByPlayCountDesc();
}
