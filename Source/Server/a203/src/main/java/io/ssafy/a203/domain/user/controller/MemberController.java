package io.ssafy.a203.domain.user.controller;

import io.ssafy.a203.domain.user.dto.request.ModifyRequestDTO;
import io.ssafy.a203.domain.user.dto.request.SignupRequestDTO;
import io.ssafy.a203.domain.user.dto.request.UpdateStatsRequestDTO;
import io.ssafy.a203.domain.user.dto.response.LeaderboardResponseDTO;
import io.ssafy.a203.domain.user.dto.response.MemberResponseDTO;
import io.ssafy.a203.domain.user.service.MemberService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.data.repository.query.ReactiveQueryByExampleExecutor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@Slf4j
@RequiredArgsConstructor
@RequestMapping("/api/user/")
public class MemberController {

    private final MemberService memberService;

    @PostMapping("/signup")
    public ResponseEntity<String> signup(@RequestBody SignupRequestDTO signupRequestDTO) throws Exception {
        memberService.signUp(SignupRequestDTO.toEntity(signupRequestDTO));
        return new ResponseEntity<>("가입에 성공", HttpStatus.CREATED);
    }

    @GetMapping("/find")
    public MemberResponseDTO find(@RequestParam String puid) throws Exception{
        return MemberResponseDTO.toDTO(memberService.findByPuid(puid));
    }

    @PostMapping("/modify")
    public ResponseEntity<String> modify(@RequestBody ModifyRequestDTO modifyRequestDTO) throws Exception{
        memberService.modifyNickname(ModifyRequestDTO.toEntity(modifyRequestDTO));
        return new ResponseEntity<>("가입에 성공", HttpStatus.OK);
    }

    @GetMapping("/rank")
    public ResponseEntity<List<LeaderboardResponseDTO>> rank(@RequestParam String stat) throws Exception{
        return new ResponseEntity<>(LeaderboardResponseDTO.toDTO(memberService.rank(stat), stat), HttpStatus.OK);
    }

    @PostMapping("/update")
    public ResponseEntity<String> update(@RequestBody UpdateStatsRequestDTO updateStatsRequestDTO) throws Exception{
        memberService.update(UpdateStatsRequestDTO.toEntity(updateStatsRequestDTO));
        return new ResponseEntity<>("업데이트 성공", HttpStatus.OK);
    }
}
