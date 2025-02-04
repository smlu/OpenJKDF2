#include "rdPuppet.h"

#include <math.h>

#include "General/stdMath.h"
#include "Engine/rdroid.h"
#include "stdPlatform.h"
#include "jk.h"

rdPuppet* rdPuppet_New(rdThing *thing)
{
    rdPuppet* puppet = (rdPuppet *)rdroid_pHS->alloc(sizeof(rdPuppet));

    if (!puppet )
        return NULL;

    // Added: Moved this memset after the nullptr check
    _memset(puppet, 0, sizeof(rdPuppet));

    puppet->paused = 0;
    puppet->rdthing = thing;

    for (int i = 0; i < 4; i++)
    {
        puppet->tracks[i].field_120 = 0.0;
        puppet->tracks[i].field_124 = 0.0;
        if ( puppet->tracks[i].callback )
        {
            puppet->tracks[i].callback(puppet->rdthing->parentSithThing, i, 0);
        }
        puppet->tracks[i].field_4 = 0;
        puppet->tracks[i].keyframe = NULL;
        puppet->tracks[i].callback = NULL;
    }
    thing->puppet = puppet;
    return puppet;
}

void rdPuppet_Free(rdPuppet *puppet)
{
    // Moved: no nullptr deref
    if (!puppet) return;

    // Added: prevent UAFs
    for (int i = 0; i < 4; i++)
    {
        puppet->tracks[i].field_4 = 0;
        puppet->tracks[i].keyframe = NULL;
        puppet->tracks[i].callback = NULL;
    }
    
    rdroid_pHS->free(puppet);
}

void rdPuppet_BuildJointMatrices(rdThing *thing, rdMatrix34 *matrix)
{
    rdModel3 *model_; // edx
    rdPuppet *puppet; // eax
    rdPuppetTrack *v4; // ebx
    rdKeyframe *v5; // ecx
    rdJoint *v6; // edx
    int v7; // ecx
    rdJoint *v8; // esi
    int v9; // edi
    uint32_t v10; // eax
    rdAnimEntry* v11; // edx
    intptr_t v12; // ecx
    rdAnimEntry *v13;
    rdHierarchyNode *v15; // edi
    rdPuppetTrack *v16; // esi
    rdKeyframe *v17; // ebp
    int v18; // ebx
    unsigned int v19; // ecx
    rdJoint *v20; // edx
    uint32_t v21; // eax
    rdAnimEntry *v22; // ecx
    double v23; // st7
    rdAnimEntry *v24; // eax
    int v25; // ecx
    double v26; // st5
    double v27; // st6
    double v28; // st5
    double v29; // st6
    double v30; // st7
    float *v31; // eax
    double v32; // st3
    double v33; // st7
    double v34; // rt2
    double v35; // rtt
    double v36; // st4
    double v37; // st7
    double v40; // st6
    double v41; // st4
    float v42; // edx
    float v43; // eax
    double v44; // st4
    double v45; // st7
    double v46; // st7
    double v47; // st6
    double v48; // st5
    double v49; // st7
    double v50; // st7
    double v51; // st7
    double v52; // st6
    double v53; // st5
    double v54; // st4
    double v55; // st3
    double v56; // st2
    double v57; // st7
    double v58; // st2
    double v59; // st6
    rdMatrix34 *v60; // eax
    rdVector3 *v61; // ecx
    float v69; // [esp+0h] [ebp-84h]
    float v70; // [esp+14h] [ebp-70h]
    float v71; // [esp+18h] [ebp-6Ch]
    uint32_t* v72; // [esp+1Ch] [ebp-68h]
    int v73; // [esp+1Ch] [ebp-68h]
    int v74; // [esp+20h] [ebp-64h]
    int v75; // [esp+20h] [ebp-64h]
    int v76; // [esp+24h] [ebp-60h]
    int v77; // [esp+24h] [ebp-60h]
    rdModel3 *model; // [esp+28h] [ebp-5Ch]
    int v80; // [esp+30h] [ebp-54h]
    int v81; // [esp+34h] [ebp-50h]
    int v82; // [esp+34h] [ebp-50h]
    rdPuppetTrack *v83; // [esp+38h] [ebp-4Ch]
    rdVector3 a3; // [esp+3Ch] [ebp-48h] BYREF
    rdVector3 a4; // [esp+48h] [ebp-3Ch] BYREF
    float v86; // [esp+54h] [ebp-30h]
    float v87; // [esp+58h] [ebp-2Ch]
    float v88; // [esp+5Ch] [ebp-28h]
    rdVector3 v89; // [esp+60h] [ebp-24h]
    rdVector3 v90; // [esp+6Ch] [ebp-18h]
    rdVector3 v91; // [esp+78h] [ebp-Ch]

    model_ = thing->model3;
    puppet = thing->puppet;
    model = model_;
    if ( thing->field_18 )
    {
        return;
    }

    if ( !puppet || puppet->paused )
    {
        for (int i = 0; i < model_->numHierarchyNodes; i++)
        {
            _memcpy(&thing->hierarchyNodeMatrices[i], &model_->hierarchyNodes[i].posRotMatrix, sizeof(rdMatrix34));
        }
    }
    else
    {
        v74 = 4;
        v83 = puppet->tracks;
        v4 = puppet->tracks;
        do
        {
            // Added: joints check
            if ( v4->status && v4->keyframe && v4->keyframe->joints)
            {
                v5 = v4->keyframe;
                v6 = v5->joints;
                v7 = v5->numJoints2;
                if ( v7 )
                {
                    v8 = v6;
                    v72 = &v4->nodes;
                    v76 = v7;
                    do
                    {
                        v9 = 0;
                        if ( v8->numAnimEntries )
                        {
                            // Added: this spot keeps crashing, add bounds checks
                            if (v8->nodeIdx < 0 || v8->nodeIdx > 64)
                            {
                                v8->nodeIdx = 0;
                            }
                            v81 = v8->numAnimEntries - 1;
                            v10 = v4->nodes[v8->nodeIdx];// nodeIdx
                            if ( v10 != v81 )
                            {
                                v11 = v8->animEntries;
                                v12 = v10 + 1;
                                if ( v4->field_120 >= (double)*(float *)&v11[v10 + 1].frameNum )
                                {
                                    v13 = &v11[v10 + 2];
                                    do
                                    {
                                        if ( v12 == v81 )
                                        {
                                            v9 = 1;
                                        }
                                        else if ( v4->field_120 >= (double)*(float*)&v13->frameNum )
                                        {
                                            ++v12;
                                            v13++;
                                        }
                                        else
                                        {
                                            v9 = 1;
                                        }
                                    }
                                    while ( !v9 );
                                    *v72 = v12;
                                }
                            }
                        }
                        v8++;
                        ++v72;
                    }
                    while (v76-- != 1);
                }
            }
            ++v4;
            --v74;
        }
        while ( v74 );
        v82 = 0;
        v77 = 0;
        for (v80 = 0; v80 < model->numHierarchyNodes; v80++)
        {
            v15 = &model->hierarchyNodes[v80];
            v16 = v83;
            v75 = 0;
            v73 = 0;
            v70 = 0.0;
            v71 = 0.0;
            a4.x = 0.0;
            a4.y = 0.0;
            a4.z = 0.0;
            a3.x = 0.0;
            v90.x = 0.0;
            v91.x = 0.0;
            a3.y = 0.0;
            v90.y = 0.0;
            v91.y = 0.0;
            a3.z = 0.0;
            v90.z = 0.0;
            v91.z = 0.0;
            for (int j = 0; j < 4; j++)
            {
                v17 = v16->keyframe;
                if ( v17 )
                {
                    v18 = (v17->type & v15->type) != 0 ? v16->highPri : v16->lowPri;
                    if ( (v16->status & 2) != 0 )
                    {
                        v19 = v15->idx;
                        v20 = &v17->joints[v19]; // overflow in orig? added (moved): v19 < v17->numJoints2
                        if ( v19 < v17->numJoints2 && v20->numAnimEntries )
                        {
                            if ( v18 >= v73 && (v18 >= v75 || v70 < 1.0) && v19 < v17->numJoints2 )
                            {
                                v21 = v16->nodes[v19];
                                v22 = v20->animEntries;
                                v23 = v16->field_120 - *(float *)&v22[v21].frameNum;
                                v24 = &v22[v21];
                                v25 = v24->flags;
                                if ( (v25 & 1) != 0 )
                                {
                                    v26 = v24->vel.y * v23 + v24->pos.y;
                                    v27 = v24->vel.z * v23 + v24->pos.z;
                                    v89.x = v24->vel.x * v23 + v24->pos.x;
                                    v89.y = v26;
                                    v89.z = v27;
                                }
                                else
                                {
                                    v89 = v24->pos;
                                }
                                if ( (v25 & 2) != 0 )
                                {
                                    v28 = v24->angVel.y * v23 + v24->orientation.y;
                                    v29 = v24->angVel.z * v23 + v24->orientation.z;
                                    v86 = v24->angVel.x * v23 + v24->orientation.x;
                                    v30 = v28;
                                }
                                else
                                {
                                    v31 = &v24->orientation.x;
                                    v86 = *v31;
                                    v87 = v31[1];
                                    v30 = v87;
                                    v88 = v31[2];
                                    v29 = v88;
                                }
                                v89.x = v89.x - v15->pos.x;
                                v32 = v30;
                                v33 = v89.z - v15->pos.z;
                                v34 = v32 - v15->rot.y;
                                v35 = v29 - v15->rot.z;
                                v36 = v86 - v15->rot.x;
                                v89.y = v89.y - v15->pos.y;
                                v89.z = v33;
                                v87 = v34;
                                v88 = v35;
                                v69 = v36;
                                v86 = stdMath_NormalizeAngleAcute(v69);
                                v87 = stdMath_NormalizeAngleAcute(v87);
                                v37 = stdMath_NormalizeAngleAcute(v88);
                                v88 = v37;
                                if ( v18 > v75 ) // TODO verify
                                {
                                    v40 = v16->playSpeed;
                                    v89.x = v40 * v89.x;
                                    v89.y = v40 * v89.y;
                                    v89.z = v40 * v89.z;
                                    v86 = v40 * v86;
                                    v87 = v40 * v87;
                                    v88 = v40 * v88;
                                }
                                if ( v18 == v75 )
                                {
                                    a4.x = a4.x + v89.x;
                                    a4.y = a4.y + v89.y;
                                    v41 = v70 + v16->playSpeed;
                                    a4.z = a4.z + v89.z;
                                    a3.x = a3.x + v86;
                                    a3.y = a3.y + v87;
                                    a3.z = a3.z + v88;
                                    v70 = v41;
                                }
                                else if ( v18 <= v75 )
                                {
                                    if ( v18 <= v73 )
                                    {
                                        v90.x = v90.x + v89.x;
                                        v90.y = v90.y + v89.y;
                                        v44 = v71 + v16->playSpeed;
                                        v90.z = v90.z + v89.z;
                                        v91.x = v91.x + v86;
                                        v91.y = v91.y + v87;
                                        v91.z = v91.z + v88;
                                        v71 = v44;
                                    }
                                    else
                                    {
                                        v90 = v89;
                                        v91.x = v86;
                                        v43 = v16->playSpeed;
                                        v91.y = v87;
                                        v91.z = v88;
                                        v71 = v43;
                                        v73 = v18;
                                    }
                                }
                                else
                                {
                                    v90 = a4;
                                    v91 = a3;
                                    v71 = v70;
                                    v73 = v75;
                                    a4 = v89;
                                    a3.x = v86;
                                    v42 = v16->playSpeed;
                                    v75 = v18;
                                    a3.y = v87;
                                    a3.z = v88;
                                    v70 = v42;
                                }
                            }
                        }
                    }
                }
                ++v16;
            }
            if ( v70 >= 1.0 || v71 <= 0.0 )
            {
                if ( v70 <= 1.0 )
                    goto LABEL_58;
                v50 = 1.0 / v70;
                a4.x = v50 * a4.x;
                a4.y = v50 * a4.y;
                a4.z = v50 * a4.z;
                a3.x = v50 * a3.x;
                v47 = v50 * a3.y;
                v49 = v50 * a3.z;
            }
            else
            {
                if ( v71 > 1.0 )
                {
                    v45 = 1.0 / v71;
                    v90.x = v45 * v90.x;
                    v90.y = v45 * v90.y;
                    v90.z = v45 * v90.z;
                    v91.x = v45 * v91.x;
                    v91.y = v45 * v91.y;
                    v91.z = v45 * v91.z;
                }
                v46 = 1.0 - v70;
                a4.x = v46 * v90.x + a4.x;
                a4.y = v46 * v90.y + a4.y;
                v47 = v46 * v91.y + a3.y;
                a4.z = v46 * v90.z + a4.z;
                v48 = v46 * v91.x + a3.x;
                v49 = v46 * v91.z + a3.z;
                a3.x = v48;
            }
            a3.y = v47;
            a3.z = v49;
LABEL_58:
            a3.x = stdMath_NormalizeAngleAcute(a3.x);
            a3.y = stdMath_NormalizeAngleAcute(a3.y);
            v51 = stdMath_NormalizeAngleAcute(a3.z);
            v52 = v15->pos.x;
            v53 = v15->pos.y;
            v54 = v15->pos.z;
            v55 = v15->rot.x;
            v56 = v51;
            v57 = v15->rot.y;
            a3.z = v56;
            v58 = v52;
            v59 = v15->rot.z;
            v60 = thing->hierarchyNodeMatrices;
            a4.x = v58 + a4.x;
            a4.y = v53 + a4.y;
            a4.z = v54 + a4.z;
            a3.x = v55 + a3.x;
            a3.y = v57 + a3.y;
            a3.z = v59 + a3.z;
            rdMatrix_Build34(&v60[v82], &a3, &a4);
            v61 = &thing->hierarchyNodes2[v77];
            if ( v61->x != 0.0 || v61->y != 0.0 || v61->z != 0.0 )
                rdMatrix_PreRotate34(&thing->hierarchyNodeMatrices[v82], &thing->hierarchyNodes2[v77]);
            v15++;
            ++v77;
            ++v82;
        }
    }
    rdThing_AccumulateMatrices(thing, model->hierarchyNodes, matrix);
    thing->frameTrue = rdroid_frameTrue;
}

int rdPuppet_ResetTrack(rdPuppet *puppet, int trackNum)
{
    if ( puppet->tracks[trackNum].callback )
        puppet->tracks[trackNum].callback(puppet->rdthing->parentSithThing, trackNum, 0);
    puppet->tracks[trackNum].status = 0;
    puppet->tracks[trackNum].keyframe = 0;
    puppet->tracks[trackNum].callback = 0;
    return 1;
}

int rdPuppet_UpdateTracks(rdPuppet *puppet, float a2)
{
    //return _rdPuppet_UpdateTracks(puppet, a2);
    
    rdPuppetTrack *v3; // esi
    int v13; // [esp+14h] [ebp-4h]

    v13 = 0;
    if (puppet->paused)
        return 0;

    for (uint32_t v2 = 0; v2 < 4; v2++)
    {
        rdPuppetTrack* track = &puppet->tracks[v2];
        if (!track->status)
            continue;

        ++v13;
        if ( (track->status & 0x10) == 0 )
        {
            rdPuppet_AdvanceTrack(puppet, v2, track->speed * a2);
        }

        if (track->status & 4)
        {
            track->playSpeed = track->fadeSpeed * a2 + track->playSpeed;
            if ( track->playSpeed >= 1.0 ) // TODO verify
            {
                //printf("asdf1\n");
                track->playSpeed = 1.0;
                track->status &= ~0x4;
            }
        }
        else if (track->status & 8)
        {
            track->playSpeed = track->playSpeed - track->fadeSpeed * a2;
            if ( track->playSpeed <= 0.0 ) // TODO verify
            {
                //printf("asdf2\n");
                if ( (track->status & 0x100) != 0 )
                {
                    track->status &= ~0x8u;
                    track->status |= 0x10;
                }
                else
                {
                    if ( track->callback )
                        track->callback(puppet->rdthing->parentSithThing, v2, 0);
                    track->status = 0;
                    track->keyframe = 0;
                    track->callback = 0;
                }
            }
        }
    }

    return v13;
}

int rdPuppet_AddTrack(rdPuppet *puppet, rdKeyframe *keyframe, int lowPri, int highPri)
{
    rdPuppetTrack *v4; // ecx
    unsigned int newTrackIdx; // esi
    rdPuppetTrack *v6; // eax
    unsigned int result; // eax
    rdPuppetTrack *newTrack; // edx
    int v10; // ecx
    rdThing *v11; // eax

    v4 = puppet->tracks;
    newTrackIdx = 0;
    v6 = puppet->tracks;
    do
    {
        if ( !v6->status )
            break;
        ++newTrackIdx;
        ++v6;
    }
    while ( newTrackIdx < 4 );
    if ( newTrackIdx >= 4 )
    {
        newTrackIdx = 0;
        while ( (v4->status & 8) == 0 || (v4->status & 0x140) != 0 )
        {
            ++newTrackIdx;
            ++v4;
            if ( newTrackIdx >= 4 )
                goto LABEL_13;
        }

        if ( puppet->tracks[newTrackIdx].callback )
            puppet->tracks[newTrackIdx].callback(puppet->rdthing->parentSithThing, newTrackIdx, 0);

        puppet->tracks[newTrackIdx].status = 0;
        puppet->tracks[newTrackIdx].keyframe = 0;
        puppet->tracks[newTrackIdx].callback = 0;
LABEL_13:
        if ( newTrackIdx >= 4 )
            return -1;
    }
    newTrack = &puppet->tracks[newTrackIdx];
    newTrack->speed = keyframe->fps;
    newTrack->keyframe = keyframe;
    newTrack->highPri = highPri;
    v10 = newTrack->status;
    newTrack->lowPri = lowPri;
    v11 = puppet->rdthing;
    newTrack->status = v10 | 1;
    newTrack->playSpeed = 0.0;
    
    // Added: Added in Grim Fandango, bounds checking
    if (v11->model3->numHierarchyNodes < 0x40)
        _memset(puppet->tracks[newTrackIdx].nodes, 0, sizeof(int) * v11->model3->numHierarchyNodes);
    else
        _memset(puppet->tracks[newTrackIdx].nodes, 0, sizeof(puppet->tracks[newTrackIdx].nodes));
    result = newTrackIdx;
    newTrack->field_120 = 0.0;
    newTrack->field_124 = 0.0;
    newTrack->status = 3;
    return result;
}

void rdPuppet_SetCallback(rdPuppet *a1, int trackNum, void *callback)
{
    a1->tracks[trackNum].callback = callback;
}

int rdPuppet_FadeInTrack(rdPuppet *puppet, int trackNum, float speed)
{
    puppet->tracks[trackNum].status = puppet->tracks[trackNum].status & ~8u | 6;
    if ( speed <= 0.0 )
    {
        puppet->tracks[trackNum].fadeSpeed = 1.0;
        return 1;
    }
    else
    {
        puppet->tracks[trackNum].fadeSpeed = 1.0 / speed;
        return 1;
    }
}

void rdPuppet_AdvanceTrack(rdPuppet *puppet, int trackNum, float a3)
{
    //_rdPuppet_AdvanceTrack(puppet, trackNum, a3);
    //return;
    
    rdKeyframe *v4; // ecx
    rdPuppetTrack *v5; // esi
    double v6; // st7
    unsigned int v11; // ebx
    rdKeyframe *v12; // ecx
    int v20; // [esp+14h] [ebp-8h]
    float v21; // [esp+18h] [ebp-4h]
    float v22; // [esp+2Ch] [ebp+10h]

    v21 = 0.0;
    v20 = 0;
    v4 = puppet->tracks[trackNum].keyframe;
    v5 = &puppet->tracks[trackNum];
    if ( !v4 || a3 == 0.0 )
        return;
    v22 = a3 + puppet->tracks[trackNum].field_124;
    v6 = (double)v4->numFrames;
    puppet->tracks[trackNum].field_120 = v22;
    if ( v22 >= v6 )
    {
        if (v5->status & 0x20)
        {
            puppet->tracks[trackNum].field_120 = v6;
            v20 = 1;
        }
        else if (v5->status & 0x40)
        {
            puppet->tracks[trackNum].field_120 = v6;
            v5->status |= 0x10;
        }
        else if ( !(v5->status & 0x80u) )
        {
            v21 = floorf(v22 / v6);
            v11 = sizeof(uint32_t) * puppet->rdthing->model3->numHierarchyNodes;
            puppet->tracks[trackNum].field_120 = puppet->tracks[trackNum].field_120 - (double)puppet->tracks[trackNum].keyframe->numFrames * v21;
            _memset(&puppet->tracks[trackNum].nodes, 0, v11);
        }
        else
        {
            puppet->tracks[trackNum].fadeSpeed = 4.0;
            puppet->tracks[trackNum].field_120 = v6;
            v5->status &= ~0x4;
            v5->status |= 0x8;
            v5->status |= 0x10;
        }
    }
    if ( puppet->tracks[trackNum].callback )
    {
        if ( v4->numMarkers )
        {
            if ( v21 == 0.0 )
            {
                for (uint32_t v13 = 0; v13 < v4->numMarkers; v13++)
                {
                    if ( v4->markers.marker_float[v13] > (double)puppet->tracks[trackNum].field_120 )
                        break;
                    if ( v4->markers.marker_float[v13] > (double)puppet->tracks[trackNum].field_124 || puppet->tracks[trackNum].field_124 == 0.0 )
                    {
                        puppet->tracks[trackNum].callback(puppet->rdthing->parentSithThing, trackNum, v4->markers.marker_int[v13]);
                    }
                }
                goto LABEL_34;
            }
            if ( v21 <= 1.0 )
            {
                for (uint32_t v17 = 0; v17 < v4->numMarkers; v17++)
                {
                    if ( v4->markers.marker_float[v17] > (double)puppet->tracks[trackNum].field_124
                      || v4->markers.marker_float[v17] <= (double)puppet->tracks[trackNum].field_120 )
                    {
                        puppet->tracks[trackNum].callback(puppet->rdthing->parentSithThing, trackNum, v4->markers.marker_int[v17]);
                    }
                }
            }
            else
            {
                if ( !v4->numMarkers )
                    goto LABEL_34;
                for (uint32_t v15 = 0; v15 < v4->numMarkers; v15++)
                {
                    puppet->tracks[trackNum].callback(puppet->rdthing->parentSithThing, trackNum, v4->markers.marker_int[v15]);
                }
            }
        }
    }
LABEL_34:
    if ( v20 )
    {
        if ( puppet->tracks[trackNum].callback )
            puppet->tracks[trackNum].callback(puppet->rdthing->parentSithThing, trackNum, 0);
        v5->status = 0;
        puppet->tracks[trackNum].keyframe = 0;
        puppet->tracks[trackNum].callback = 0;
    }
    else
    {
        puppet->tracks[trackNum].field_124 = puppet->tracks[trackNum].field_120;
    }
}

int rdPuppet_FadeOutTrack(rdPuppet *puppet, int trackNum, float speed)
{
    puppet->tracks[trackNum].status = puppet->tracks[trackNum].status & ~4u | 8;
    if ( speed <= 0.0 )
    {
        puppet->tracks[trackNum].fadeSpeed = 1.0;
        return 1;
    }
    else
    {
        puppet->tracks[trackNum].fadeSpeed = 1.0 / speed;
        return 1;
    }
}

void rdPuppet_SetTrackSpeed(rdPuppet *puppet, int trackNum, float speed)
{
    puppet->tracks[trackNum].speed = speed;
}

int rdPuppet_SetStatus(rdPuppet *puppet, int trackNum, int status)
{
    puppet->tracks[trackNum].status |= status;
    return 1;
}

int rdPuppet_PlayTrack(rdPuppet *puppet, int trackNum)
{
    rdPuppetTrack *v2; // eax

    v2 = &puppet->tracks[trackNum];
    v2->status = v2->status & ~0x10u | 2;
    v2->playSpeed = 1.0;
    return 1;
}

void rdPuppet_unk(rdPuppet *puppet, int trackNum)
{
    rdPuppetTrack *v2; // edx

    v2 = &puppet->tracks[trackNum];
    _memset(&puppet->tracks[trackNum].nodes, 0, 4 * puppet->rdthing->model3->numHierarchyNodes);
    v2->field_120 = 0.0;
    v2->field_124 = 0.0;
    v2->status = 3;
}

int rdPuppet_RemoveTrack(rdPuppet *puppet, rdThing *rdthing)
{
    puppet->paused = 0;
    puppet->rdthing = rdthing;
    for (int i = 0; i < 4; i++)
    {
        puppet->tracks[i].field_120 = 0.0;
        puppet->tracks[i].field_124 = 0.0;
        if ( puppet->tracks[i].callback )
        {
            puppet->tracks[i].callback(puppet->rdthing->parentSithThing, i, 0);
        }
        puppet->tracks[i].status = 0;
        puppet->tracks[i].keyframe = 0;
        puppet->tracks[i].callback = 0;
    }

    return 1;
}
