// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "GithubCopilotLLMProvider.g.h"

namespace winrt::Microsoft::Terminal::Query::Extension::implementation
{
    struct GithubCopilotBranding : public winrt::implements<GithubCopilotBranding, winrt::Microsoft::Terminal::Query::Extension::IBrandingData>
    {
        GithubCopilotBranding();

        WINRT_PROPERTY(winrt::hstring, Name, L"GitHub Copilot");
        WINRT_PROPERTY(winrt::hstring, HeaderIconPath);
        WINRT_PROPERTY(winrt::hstring, HeaderText);
        WINRT_PROPERTY(winrt::hstring, SubheaderText);
        WINRT_PROPERTY(winrt::hstring, BadgeIconPath);
        WINRT_PROPERTY(winrt::hstring, ResponseMetaData);
        WINRT_PROPERTY(winrt::hstring, QueryMetaData);
    };

    struct GithubCopilotAuthenticationResult : public winrt::implements<GithubCopilotAuthenticationResult, winrt::Microsoft::Terminal::Query::Extension::IAuthenticationResult>
    {
        GithubCopilotAuthenticationResult(const winrt::hstring& errorMessage, const Windows::Foundation::Collections::ValueSet& authValues) :
            ErrorMessage{ errorMessage },
            AuthValues{ authValues } {}

        til::property<winrt::hstring> ErrorMessage;
        til::property<Windows::Foundation::Collections::ValueSet> AuthValues;
    };

    struct GithubCopilotLLMProvider : GithubCopilotLLMProviderT<GithubCopilotLLMProvider>
    {
        GithubCopilotLLMProvider() = default;

        void ClearMessageHistory();
        void SetSystemPrompt(const winrt::hstring& systemPrompt);
        void SetContext(const Extension::IContext context);

        winrt::Windows::Foundation::IAsyncOperation<Extension::IResponse> GetResponseAsync(const winrt::hstring& userPrompt);

        void SetAuthentication(const Windows::Foundation::Collections::ValueSet& authValues);
        TYPED_EVENT(AuthChanged, winrt::Microsoft::Terminal::Query::Extension::ILMProvider, winrt::Microsoft::Terminal::Query::Extension::IAuthenticationResult);

        WINRT_PROPERTY(IBrandingData, BrandingData, winrt::make<GithubCopilotBranding>());

    private:
        winrt::hstring _authToken;
        winrt::hstring _refreshToken;
        winrt::hstring _endpointUri;
        winrt::Windows::Web::Http::HttpClient _httpClient{ nullptr };
        winrt::hstring _lastResponse;

        Extension::IContext _context;

        winrt::Windows::Data::Json::JsonArray _jsonMessages;

        void _refreshAuthTokens();
        safe_void_coroutine _completeAuthWithUrl(const Windows::Foundation::Uri url);
        safe_void_coroutine _obtainUsernameAndRefreshTokensIfNeeded();
        winrt::Windows::Data::Json::JsonObject _SendRequestReturningJson(std::wstring_view uri, const winrt::Windows::Web::Http::IHttpContent& content = nullptr, winrt::Windows::Web::Http::HttpMethod method = nullptr);
    };

    struct GithubCopilotResponse : public winrt::implements<GithubCopilotResponse, winrt::Microsoft::Terminal::Query::Extension::IResponse>
    {
        GithubCopilotResponse(const winrt::hstring& message, const winrt::Microsoft::Terminal::Query::Extension::ErrorTypes errorType) :
            Message{ message },
            ErrorType{ errorType } {}

        til::property<winrt::hstring> Message;
        til::property<winrt::Microsoft::Terminal::Query::Extension::ErrorTypes> ErrorType;
    };
}

namespace winrt::Microsoft::Terminal::Query::Extension::factory_implementation
{
    BASIC_FACTORY(GithubCopilotLLMProvider);
}
