#include "nodefactory.h"
#include "pcmsourcecreator.h"
#include "mediatakeelement.h"
#include "mediatrackelement.h"
#include "importelement.h"
#include "objectautomationelement.h"
#include "directspeakerautomationelement.h"
#include "hoaautomationelement.h"
#include "projectnode.h"

using namespace admplug;

NodeCreator::NodeCreator(std::shared_ptr<IPCMSourceCreator> pcmCreator, MediaItem* fromMediaItem) :
    pcmCreator{std::move(pcmCreator)},
    originalMediaItem{ fromMediaItem }
{
}

std::unique_ptr<RootElement> admplug::NodeCreator::createRootElement(MediaItem* fromMediaItem) {
    return std::make_unique<ImportElement>(fromMediaItem);
}

std::shared_ptr<ProjectNode> admplug::NodeCreator::createObjectTrackNode(std::vector<adm::ElementConstVariant> elements, std::shared_ptr<TrackElement> parentGroup)
{
    return std::make_shared<ProjectNode>(std::make_unique<ObjectTrack>(elements, parentGroup));
}

std::shared_ptr<ProjectNode> NodeCreator::createDirectTrackNode(std::vector<adm::ElementConstVariant> elements, std::shared_ptr<TrackElement> parentGroupTrack)
{
    return std::make_shared<ProjectNode>(std::make_unique<DirectTrack>(elements, parentGroupTrack));
}

std::shared_ptr<ProjectNode> NodeCreator::createHoaTrackNode(std::vector<adm::ElementConstVariant> elements, std::shared_ptr<TrackElement> parentGroupTrack)
{
    return std::make_shared<ProjectNode>(std::make_unique<HoaTrack>(elements, parentGroupTrack));
}

std::shared_ptr<ProjectNode> admplug::NodeCreator::createGroupNode(std::vector<adm::ElementConstVariant> elements, std::shared_ptr<TrackElement> parentGroupTrack)
{
    auto track = std::make_unique<GroupTrack>(elements, parentGroupTrack, std::make_unique<TrackGroup>(currentGroup));
    currentGroup += 1;
    return std::make_shared<ProjectNode>(std::move(track));
}

std::shared_ptr<ProjectNode> NodeCreator::createTakeNode(std::shared_ptr<const adm::AudioObject> parentObject, std::shared_ptr<TrackElement> parentTrack, std::shared_ptr<const adm::AudioTrackUid> trackUid)
{
    auto projectElement = std::make_shared<MediaTakeElement>(parentObject, parentTrack, originalMediaItem);
    pcmCreator->addTake(projectElement);
    auto projectNode = std::make_shared<ProjectNode>(projectElement);
    return projectNode;
}

std::shared_ptr<ProjectNode> admplug::NodeCreator::createAutomationNode(ADMChannel channel, std::shared_ptr<TakeElement> parentTake)
{
    auto channelFormat = channel.channelFormat();
    if(channelFormat) {
        if(!channelFormat->getElements<adm::AudioBlockFormatDirectSpeakers>().empty()) {
            return std::make_shared<ProjectNode>(std::make_unique<DirectSpeakersAutomationElement>(channel, parentTake));
        }
        if(!channelFormat->getElements<adm::AudioBlockFormatHoa>().empty()) {
            return std::make_shared<ProjectNode>(std::make_unique<HoaAutomationElement>(channel, parentTake));
        }
    }

    // if no blocks default to object track
    return std::make_shared<ProjectNode>(std::make_unique<ObjectAutomationElement>(channel, parentTake));

}
