<button type="button" class="btn btn-primary btn-xs" aria-hidden="true" ngf-select ngf-change="onFileSelect($files)"
  translate>file.upload.button
</button>

<table ng-show="files.length" class="table table-striped">
  <tbody>
  <tr ng-repeat="file in files">
    <td>
      <progressbar ng-show="file.showProgressBar" class="progress-striped" value="file.progress">
        {{file.progress}}%
      </progressbar>
    </td>
    <td>
      {{file.fileName}}
    </td>
    <td>
      {{file.size | bytes}}
    </td>
    <td>
      <small ng-show="file.md5">md5: {{file.md5}}</small>
    </td>
    <td>
      <a ng-show="file.id" ng-click="deleteFile(file.id)" class="action">
        <i class="fa fa-trash-o"></i>
      </a>
      <a ng-show="file.tempId" ng-click="deleteTempFile(file.tempId)" class="action">
        <i class="fa fa-trash-o"></i>
      </a>
    </td>
  </tr>
  </tbody>
</table>