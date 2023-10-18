package io.ssafy.a203.domain.user.entity;

import jakarta.persistence.*;
import lombok.*;

@Getter
@Setter
@Builder
@NoArgsConstructor
@AllArgsConstructor
@Entity
@Table(name="member")
public class Member {

    @Id
    @Column
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long id;

    @Column(unique = true)
    private String puid;

    @Column
    private String nickname;

    @Column
    private int gold;

    @Column
    private int killCount;

    @Column
    private int playCount;
}
