#include "Enemy.h"
#include "DxLib.h"
#include <cstdint>
#include "Util.h"
#include "MathUtil.h"
#include "PlayerAttack.h"
#include "Vector3.h"
#include "ParticleManager.h"
#include "Score.h"
#include <cmath>

const float Enemy::kMoveSpeed_ = 30.0f;
const float Enemy::kPushBackDist_ = 2.0f;
const float Enemy::kPngScale_ = 0.07f;
const float Enemy::KRadius_ = 16.f;

Enemy::Enemy(CollisionManger* colMPtr, Player* playerPtr, Stage* stagePtr) : IEntity(stagePtr), playerPtr_(playerPtr), colMPtr_(colMPtr)
{
    // 衝突マネージャへの登録
    colMPtr->Register(this);

    // 形状設定
    shape_ = Shape::CIRCLE;
    // 名称設定
    id_ = "enemy";
    // 衝突callback反映
    onCollision_ = std::bind(&Enemy::OnCollision, this);

    // 待機フレームが生成時にばらけるように
    frameCount_wait_ = Math::Function::Random<int32_t>(5, kMoveInterval_ - 5);
}

Enemy::~Enemy(void)
{
    // 登録の抹消
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
}

void Enemy::Update(void)
{
    // 生きてるなら
    if (isAlive_)
    {
        // 前フレームから持ち越しはしない。
        //isContactMowSupport_ = false;

        // 薙ぎ払われてたら
        if (isMowDown_)
        {
            // イージングタイマーが起動されてたら
            if (frameCount_mow_)
            {
                // 加算
                Math::Function::LoopIncrement<int32_t>(frameCount_mow_, 0, kMowFrame_);
            }
            else
            {
                // 起動されてなければ起動
                frameCount_mow_++;
            }

            /* 吹っ飛び処理と移動したらisMowDownのオフ */

            // イージング用のタイムレート
            float rate = (std::min)((float)frameCount_mow_ / kMowFrame_, 1.f);

            // 吹き飛ばされる速さ を イージングで調整
            const float mowSpeed = (1 - Math::Ease::EaseInSine(rate)) * Player::kMowDist_;
            // 座標に加算
            position_ += vec_mow_ * mowSpeed;

            // 押し戻しっつーか、それ以上いかないようにってだけ
            position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
            position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);

            // 座標のあれこれ終わったので、合計を鑑みて、終わるかどうか確認します。
            // rateが1ならイージング終わってます
            if (rate >= 1.f)
            {
                //tortalMowDist = 0;
                // 吹き飛ばされるためのカウンタ初期化
                frameCount_mow_ = 0;
                isMowDown_ = false;
            }
        }
        else if (!isDocking_)// 薙ぎ払われてない時はいつも通りすごしてくれて結構
        {

            // 移動用フレームが0なら(移動が終わったなら)
            if (frameCount_move_ == 0)
            {
                // 待機用frameを加算（縮み状態）
                Math::Function::LoopIncrement<int32_t>(frameCount_wait_, 0, kMoveInterval_);
            }

            // 待機用フレームが規定値以上なら（待機が終わったなら）
            if (frameCount_wait_ >= kMoveInterval_)
            {
                // 敵(自身)からプレイヤーまでの方向ベクトル
                Vector2 vec_enemy2player = (playerPtr_->GetPos() - position_).Normalize();

                // イージング用のタイムレート
                float rate = (std::min)((float)frameCount_move_ / kMoveTimes_, 1.f);

                // 吹き飛ばされる速さ を イージングで調整
                const float speed = (1 - Math::Ease::EaseInSine(rate)) * Player::kMowDist_;
                // 座標に加算
                position_ += vec_enemy2player * speed;

                // 押し戻しっつーか、それ以上いかないようにってだけ
                position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
                position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);

                // 座標のあれこれ終わったので、合計を鑑みて、終わるかどうか確認します。
                // rateが1ならイージング終わってます
                if (rate >= 1.f)
                {
                    //tortalMowDist = 0;
                    // 吹き飛ばされるためのカウンタ初期化
                    frameCount_mow_ = 0;
                    isMowDown_ = false;
                }

                // 移動用フレームを加算（伸び状態）※最大フレーム数は移動回数と同じ
                Math::Function::LoopIncrement<int32_t>(frameCount_move_, 0, kMoveTimes_);
            }
        }
    }

    MowDownFlagUpdate();
}

void Enemy::Draw(void)
{
    // 生きてるなら
    if (isAlive_)
    {
        // 縮み状態なら
        if (frameCount_move_ == 0)
        {
            // 敵の色は赤色に
            SetDrawBright(255, 50, 50);
            DrawRotaGraph3((int32_t)position_.x, (int32_t)position_.y, 250, 250,
                scale_.x * kPngScale_, scale_.y * kPngScale_, rotation_, png_enemy_, true);
            SetDrawBright(255, 255, 255);
#ifdef _DEBUG
            DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::WHITE, false, 1);
#endif
        }

        // 伸び状態なら
        if (frameCount_wait_ >= kMoveInterval_)
        {
            // 敵の色は緑色に
            SetDrawBright(50, 255, 50);
            DrawRotaGraph3((int32_t)position_.x, (int32_t)position_.y, 250, 250,
                scale_.x * kPngScale_, scale_.y * kPngScale_, rotation_, png_enemy_, true);
            SetDrawBright(255, 255, 255);
#ifdef _DEBUG
            DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, false, 1);
#endif // _DEBUG
        }
    }
}

void Enemy::MowDownFlagUpdate()
{
    //敵の攻撃が終わったら合体しないように
    if (!playerPtr_->GetFrameCountAttack())
    {
        isMowDownTrigger_ = false;
    }
}

void Enemy::OnCollision(void)
{
    // 接触対象の名称が player_skewerAttack
    if (other_->GetId() == "player_skewerAttack")
    {
        // キャストして復元
        PlayerSkewerAttack* skewerPtr = static_cast<PlayerSkewerAttack*>(other_);
        // 串刺し攻撃中は死なない
        if (skewerPtr->GetIsSkewer())
        {
            isSkewer_ = true;
        }
        else
        {
            Dead();
        }

    }

    // 接触対象の名称が enemy
    if (other_->GetId() == "enemy")
    {
        Enemy* enemyPtr = static_cast<Enemy*>(other_);

        // 薙ぎ払われてる最中に他の敵と触れたら
        if (isMowDown_ || isSkewer_)
        {
            // 合体OKフラグオン
            isDocking_ = true;
            enemyPtr->isDocking_ = true;
            // 関数終了
            return;
        }
        //串しざされ中にほかの敵と触れたら
        if (isSkewer_)
        {
            //相手も串しざされる
            enemyPtr->isSkewer_ = true;
        }

        // 他の敵から自分までの方向ベクトル
        Vector2 vec_enemy2enemyself = (position_ - other_->GetPos()).Normalize();

        // 押し戻し後の座標 = 座標 + (正規化された押し戻し方向 * 速度)
        Vector2 pushBacked_pos = position_ + vec_enemy2enemyself * kPushBackDist_;

        // ノックバック後の座標 (+ 半径)が、ステージの内側なら座標反映
        if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
            pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
        {
            // 反映
            position_ = pushBacked_pos;
        }
    }

    // 接触対象の名称が player
    if (other_->GetId() == "player")
    {
        // キャストして復元
        Player* playerPtr = static_cast<Player*>(other_);

        // 串刺しされてる最中なら押し戻し要らん
        if (playerPtr->GetIsSkewer()) return;

        // playerから自分までの方向ベクトル
        Vector2 vec_player2enemy = (position_ - other_->GetPos()).Normalize();

        // 押し戻し後の座標 = 座標 + (正規化された押し戻し方向 * 速度)
        Vector2 pushBacked_pos = position_ + vec_player2enemy * kPushBackDist_;

        // ノックバック後の座標 (+ 半径)が、ステージの内側なら座標反映
        if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
            pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
        {
            // 反映
            position_ = pushBacked_pos;
        }
    }

    // 接触対象の名称が player_mowAttackSupport
    if (other_->GetId() == "player_mowAttackSupport") // 絶対にsupportが先に触れる。逆に触れないと正常な判定は期待できない
    {
        //isContactMowSupport_ = true;
    }

    // 接触対象の名称が player_attack && supportに既に触れているもの
    if (other_->GetId() == "player_mowAttack"/* && isContactMowSupport_*/)
    {
        // 薙ぎ払いの半円に触れている状態で、プレイヤーの向きと、プレイヤーから敵への向きの内積値が、正の値の場合、正面の半円内にいると思われる。
        if (playerPtr_->GetMoveVec().Normalize().Dot((position_ - playerPtr_->GetPos()).Normalize()) >= 0.f)
        {
            // ptrをキャストして復元
            PlayerMowAttack* paPtr = static_cast<PlayerMowAttack*>(other_);

            // 攻撃猶予中なら
            if (paPtr->GetFrameCountAttack()) //** 現状下記の条件式だと、範囲内に中心点はいないけど、半径は触れているみたいな状態が考慮されていない。やり方もわからない。
            {
                // 座標を、当たり判定円の中心(=プレイヤーの中心座標）+ プレイヤーの正面 * (プレイヤーの半径 + 5.f)に設定
                position_ = paPtr->GetPos() + paPtr->GetVecMove() * 15.f;
                //position_ = { 300,300 };

                // 薙ぎ払われたフラグオン
                isMowDown_ = true;
                isMowDownTrigger_ = true;
                // 吹き飛ばされる方向を記録
                vec_mow_ = paPtr->GetVecMove();
            }
        }
    }
}

void Enemy::Docking()
{
    isDocking_ = false;
    vec_mow_ = { 0,0 };
}

void Enemy::Dead()
{
    if (isAlive_)
    {
        ParticleManager::GetInstance()->BossHitEnemyEffect(position_);
        Score::Add(500);
    }
    isAlive_ = false;
}
