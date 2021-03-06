# coding=utf-8
import logging
import datetime

from apiclient.errors import HttpError


def watch_change(service, channel_id, channel_type, channel_address,
                 channel_token=None, channel_params=None):
    """Watch for all changes to a user's Drive.

    Args:
      service: Drive API service instance.
      channel_id: Unique string that identifies this channel.
      channel_type: Type of delivery mechanism used for this channel.
      channel_address: Address where notifications are delivered.
      channel_token: An arbitrary string delivered to the target address with
                     each notification delivered over this channel. Optional.
      channel_address: Address where notifications are delivered. Optional.

    Returns:
      The created channel if successful
    Raises:
      apiclient.errors.HttpError: if http request to create channel fails.
    """
    body = {
        'id': channel_id,
        'type': channel_type,
        'address': channel_address
    }
    if channel_token:
        body['token'] = channel_token
    if channel_params:
        body['params'] = channel_params
    return service.changes().watch(body=body).execute()


def check_modification_date(file_info):
    try:
        if file_info['modifiedDate'] == file_info['modifiedByMeDate']:
            return False
    except KeyError:
        pass
    return True


def check_is_viewed(file_info):
    try:
        modifiedDate = datetime.datetime.strptime(file_info['modifiedDate'], '%Y-%m-%dT%H:%M:%S.%fZ')
        lastViewedByMeDate = datetime.datetime.strptime(file_info['lastViewedByMeDate'], '%Y-%m-%dT%H:%M:%S.%fZ')
        if lastViewedByMeDate >= modifiedDate:
            return False
    except KeyError:
        pass
    return True


def filter_change(file_info):
    return check_modification_date(file_info) and check_is_viewed(file_info)


def retrieve_change(service, change_id):
    """
    Print a single Change resource information.

    Args:
      service: Drive API service instance.
      change_id: ID of the Change resource to retrieve.
    """
    result = {}
    try:
        change = service.changes().get(changeId=change_id).execute()
        if not change.get('file'):
            return {}
        file_info = change['file']
        if not filter_change(file_info):
            return {}

        result['modificationDate'] = change['modificationDate']
        result['title'] = file_info['title']
        result['URL'] = file_info['alternateLink']
        if file_info.get('lastModifyingUser'):
            user = file_info['lastModifyingUser']
            result['user_name'] = "Неизвестный пользователь"
            if user.get('displayName'):
                result['user_name'] = user['displayName']
            if user.get('picture') and user['picture'].get('url'):
                result['picture'] = user['picture']['url']
    except HttpError, error:
        logging.warning('Error occurred while retrieve changes:')
        logging.warning(error)
    return result


def stop_channel(service, channel_id, resource_id):
    """Stop watching to a specific channel.

    Args:
      service: Drive API service instance.
      channel_id: ID of the channel to stop.
      resource_id: Resource ID of the channel to stop.
    Raises:
      apiclient.errors.HttpError: if http request to create channel fails.
    """
    body = {
        'id': channel_id,
        'resourceId': resource_id
    }
    service.channels().stop(body=body).execute()
