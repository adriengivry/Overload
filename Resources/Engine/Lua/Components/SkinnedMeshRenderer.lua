---@meta

--- A component responsible for skeletal animation playback and skinning runtime control
--- Any number of animations can play at once, each layer owning its own animation source model,
--- animation, time, speed, loop mode and weight.
--- Layer arguments default to 0 (the base layer), which always exists
---@class SkinnedMeshRenderer : Component
SkinnedMeshRenderer = {}

--- Returns the actor that owns this component
---@return Actor
function SkinnedMeshRenderer:GetOwner() end

--- Returns the number of active animation layers (always at least 1)
---@return integer
function SkinnedMeshRenderer:GetLayerCount() end

--- Appends a new animation layer and returns its index
---@return integer
function SkinnedMeshRenderer:AddLayer() end

--- Removes an animation layer, shifting the following layers down by one.
--- Fails when the index is invalid or when only one layer remains
---@param layer integer
---@return boolean
function SkinnedMeshRenderer:RemoveLayer(layer) end

--- Starts/resumes animation playback on a layer
---@param layer? integer
function SkinnedMeshRenderer:Play(layer) end

--- Pauses animation playback on a layer
---@param layer? integer
function SkinnedMeshRenderer:Pause(layer) end

--- Stops playback on a layer and resets its time to start
---@param layer? integer
function SkinnedMeshRenderer:Stop(layer) end

--- Returns whether playback is active on a layer
---@param layer? integer
---@return boolean
function SkinnedMeshRenderer:IsPlaying(layer) end

--- Sets looping mode on a layer
---@param loop boolean
---@param layer? integer
function SkinnedMeshRenderer:SetLooping(loop, layer) end

--- Returns whether looping is enabled on a layer
---@param layer? integer
---@return boolean
function SkinnedMeshRenderer:IsLooping(layer) end

--- Sets animation playback speed on a layer
---@param speed number
---@param layer? integer
function SkinnedMeshRenderer:SetPlaybackSpeed(speed, layer) end

--- Returns the playback speed of a layer
---@param layer? integer
---@return number
function SkinnedMeshRenderer:GetPlaybackSpeed(layer) end

--- Sets the blend weight of a layer, clamped to the [0,1] range
--- Weights are normalized per bone against the other layers animating that bone, so a layer
--- animating a bone alone contributes fully until its weight reaches 0, where the bone falls
--- back to its bind pose
---@param weight number
---@param layer? integer
function SkinnedMeshRenderer:SetLayerWeight(weight, layer) end

--- Returns the blend weight of a layer
---@param layer? integer
---@return number
function SkinnedMeshRenderer:GetLayerWeight(layer) end

--- Sets the bounds scale used during frustum culling for skinned meshes
--- Values below 1.0 are clamped to 1.0
---@param scale number
function SkinnedMeshRenderer:SetMeshBoundsScale(scale) end

--- Returns the bounds scale used during frustum culling for skinned meshes
--- Returned value is always >= 1.0
---@return number
function SkinnedMeshRenderer:GetMeshBoundsScale() end

--- Sets the playback time of a layer in seconds
---@param timeSeconds number
---@param layer? integer
function SkinnedMeshRenderer:SetTime(timeSeconds, layer) end

--- Returns the playback time of a layer in seconds
---@param layer? integer
---@return number
function SkinnedMeshRenderer:GetTime(layer) end

--- Sets the external model used as animation source by a layer, or nil to use the rendered model
---@param model Model|nil
---@param layer? integer
function SkinnedMeshRenderer:SetAnimationSourceModel(model, layer) end

--- Returns the external animation source model of a layer, or nil when the rendered model is used
---@param layer? integer
---@return Model|nil
function SkinnedMeshRenderer:GetAnimationSourceModel(layer) end

--- Returns whether the animation source of a layer is compatible with the rendered model skeleton
---@param layer? integer
---@return boolean
function SkinnedMeshRenderer:IsAnimationSourceCompatible(layer) end

--- Returns the number of animation clips available to a layer
---@param layer? integer
---@return integer
function SkinnedMeshRenderer:GetAnimationCount(layer) end

--- Returns the clip name at index for a layer, or nil
---@param index integer
---@param layer? integer
---@return string|nil
function SkinnedMeshRenderer:GetAnimationName(index, layer) end

--- Sets the active animation clip of a layer by index or name, returns true on success
---@overload fun(self: SkinnedMeshRenderer, name: string, layer?: integer): boolean
---@overload fun(self: SkinnedMeshRenderer, index: integer|nil, layer?: integer): boolean
---@return boolean
function SkinnedMeshRenderer:SetAnimation(...) end

--- Returns the current clip index of a layer, or nil
---@param layer? integer
---@return integer|nil
function SkinnedMeshRenderer:GetActiveAnimationIndex(layer) end

--- Returns the current clip name of a layer, or nil
---@param layer? integer
---@return string|nil
function SkinnedMeshRenderer:GetActiveAnimationName(layer) end

--- Returns the number of available bones
---@return integer
function SkinnedMeshRenderer:GetBoneCount() end

--- Returns bone name at index or nil
---@param index integer
---@return string|nil
function SkinnedMeshRenderer:GetBoneName(index) end

--- Returns bone index by name or nil
---@param name string
---@return integer|nil
function SkinnedMeshRenderer:GetBoneIndex(name) end

--- Returns local bone position or nil
---@param boneIndex integer
---@return Vector3|nil
function SkinnedMeshRenderer:GetBoneLocalPosition(boneIndex) end

--- Returns local bone rotation or nil
---@param boneIndex integer
---@return Quaternion|nil
function SkinnedMeshRenderer:GetBoneLocalRotation(boneIndex) end

--- Returns local bone scale or nil
---@param boneIndex integer
---@return Vector3|nil
function SkinnedMeshRenderer:GetBoneLocalScale(boneIndex) end

--- Sets local bone position
---@param boneIndex integer
---@param position Vector3
---@return boolean
function SkinnedMeshRenderer:SetBoneLocalPosition(boneIndex, position) end

--- Sets local bone rotation
---@param boneIndex integer
---@param rotation Quaternion
---@return boolean
function SkinnedMeshRenderer:SetBoneLocalRotation(boneIndex, rotation) end

--- Sets local bone scale
---@param boneIndex integer
---@param scale Vector3
---@return boolean
function SkinnedMeshRenderer:SetBoneLocalScale(boneIndex, scale) end
